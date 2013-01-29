#include <string.h>
#include <Arduino.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"

const COLOR Game::kPuyoColors[kMaxColorNum] = {
    CLEAR, RED, GREEN, BLUE, MAGENTA, YELLOW, CYAN, WHITE,
};

const int Game::kDxs[4] = {0, 1, 0, -1};
const int Game::kDys[4] = {-1, 0, 1, 0};
const int Game::kDds[4] = {2, 3, 0, 1};

Game::Game(SNESpad *p_nintendo) {
    p_nintendo_ = p_nintendo;

    jammer_puyo_cycle_ = 8;
    clock_cycle_millis_ = 1000;
    input_clock_cycle_millis_ = 250;
    down_input_clock_cycle_millis_ = 100;
}

Game::~Game() {
}

void Game::CreatePuyo(int *puyos) {
    puyos[0] = random(1, kColorNum + 1);
    puyos[1] = random(1, kColorNum + 1);
}


void Game::SetPuyo() {
    // current <= next
    memset(&puyo_.field_, 0, sizeof(puyo_.field_));
    puyo_.field_[0][1] = next_puyos[0];
    puyo_.field_[1][1] = next_puyos[1];
    // next <= next2
    memcpy(next_puyos, next2_puyos, sizeof(next_puyos));
    // create next2
    CreatePuyo(next2_puyos);

    // ぷよをフィールドに配置
    puyo_.x_ = 2;
    puyo_.y_ = 0;
    for (int y = 0; y < kPuyoHeight; y++) {
        for (int x = 0; x < kPuyoWidth; x++) {
            field_float_[puyo_.y_ + y][puyo_.x_ + x] += puyo_.field_[y][x];

            // 重なっていたらゲームオーバー
            if (puyo_.field_[y][x] && field_float_[puyo_.y_ + y][puyo_.x_ + x] != puyo_.field_[y][x]) {
                is_over_ = true;
                return;
            }
        }
    }
}

void Game::SetJammerPuyo() {
    for (int x = 1; x < kFieldWidth - 1; x++) {
        if (field_float_[0][x] == 0) {
            field_float_[0][x] = kMaxColorNum - 1;
        }
    }
    FallPuyo();
}

bool Game::CheckOverlap(int dx, int dy) {
    int nx = puyo_.x_ + dx;
    int ny = puyo_.y_ + dy;
    if (nx < 0) {
        return true;
    }
    for (int y = 0; y < kPuyoHeight; y++) {
        for (int x = 0; x < kPuyoWidth; x++) {
            if (puyo_.field_[y][x] && field_fixed_[ny + y][nx + x] != 0) {
                return true;
            }
        }
    }
    return false;
}

void Game::MovePuyo(int dx, int dy) {
    memcpy(field_float_, field_fixed_, sizeof(field_float_));

    puyo_.x_ += dx;
    puyo_.y_ += dy;
    for (int y = 0; y < kPuyoHeight; y++) {
        for (int x = 0; x < kPuyoWidth; x++) {
            field_float_[puyo_.y_ + y][puyo_.x_ + x] += puyo_.field_[y][x];
        }
    }
}

void Game::TurnPuyo(bool clockwise) {
    int temp_field[kPuyoHeight][kPuyoWidth];
    memcpy(temp_field, puyo_.field_, sizeof(temp_field));

    if (clockwise) {
        int temp_value = puyo_.field_[0][1];
        puyo_.field_[0][1] = puyo_.field_[1][0];
        puyo_.field_[1][0] = puyo_.field_[2][1];
        puyo_.field_[2][1] = puyo_.field_[1][2];
        puyo_.field_[1][2] = temp_value;
    } else {
        int temp_value = puyo_.field_[0][1];
        puyo_.field_[0][1] = puyo_.field_[1][2];
        puyo_.field_[1][2] = puyo_.field_[2][1];
        puyo_.field_[2][1] = puyo_.field_[1][0];
        puyo_.field_[1][0] = temp_value;
    }

    if (CheckOverlap(0, 0)) {
        if (puyo_.field_[1][0] && !CheckOverlap(1, 0)) {
            // 左側にぷよ -> 右に移動
            MovePuyo(1, 0);
        } else if (puyo_.field_[1][2] && !CheckOverlap(-1, 0)) {
            // 左側にぷよ -> 右に移動
            MovePuyo(-1, 0);
        } else {
            // 回さなかったことにする
            memcpy(puyo_.field_, temp_field, sizeof(puyo_.field_));
        }
    } else {
        // 回転を反映させる
        MovePuyo(0, 0);
    }
}

void Game::ControlPuyo(int input) {
    if (input & SNES_A) {
        TurnPuyo(true);
    } else if (input & SNES_B) {
        TurnPuyo(false);
    } else if (input & SNES_LEFT) {
        if (!CheckOverlap(-1, 0)) {
            MovePuyo(-1, 0);
        }
    } else if (input & SNES_RIGHT) {
        if (!CheckOverlap(1, 0)) {
            MovePuyo(1, 0);
        }
    } else if (input & SNES_DOWN) {
        if (!CheckOverlap(0, 1)) {
            MovePuyo(0, 1);
        }
    }
}

void Game::FallPuyo() {
    while (DownOneRow()) {
        delay(kFallClockMillis);
        Show();
    }
    memcpy(field_fixed_, field_float_, sizeof(field_fixed_));
}

bool Game::DownOneRow() {
    bool result = false;
    for (int y = kFieldHeight - 2; y > 0; y--) {
        for (int x = 1; x < kFieldWidth - 1; x++) {
            if (field_float_[y][x] == 0 && field_float_[y - 1][x] != 0) {
                field_float_[y][x] = field_float_[y - 1][x];
                field_float_[y - 1][x] = 0;
                result = true;
            }
        }
    }
    return result;
}

bool Game::SearchAndDeletePuyo() {
    bool result = false;
    for (int y = 0; y < kFieldHeight - 1; y++) {
        for (int x = 1; x < kFieldWidth - 1; x++) {
            if (field_float_[y][x] == 0) {
                continue;
            }
            if ((x + 1 < kFieldWidth - 1 && field_float_[y][x] == field_float_[y][x + 1])
                || (y + 1 < kFieldHeight - 1 && field_float_[y][x] == field_float_[y + 1][x])) {
                result |= DeletePuyo(x, y);
            }
        }
    }
    return result;
}

bool Game::DeletePuyo(int cx, int cy) {
    memset(field_check_, 0, sizeof(field_check_));
    RecCheckNeighboring(cx, cy, field_float_[cy][cx], -1);

    int counter = 0;
    for (int y = 0; y < kFieldHeight - 1; y++) {
        for (int x = 1; x < kFieldWidth - 1; x++) {
            if (field_check_[y][x] == 1) {
                counter++;
            }
        }
    }
    if (counter < kDeletePuyoNum) {
        return false;
    }

    for (int y = 0; y < kFieldHeight - 1; y++) {
        for (int x = 1; x < kFieldWidth - 1; x++) {
            if (field_check_[y][x] == 1 || field_check_[y][x] == 2) {
                field_float_[y][x] = 0;
            }
        }
    }
    return true;
}

void Game::RecCheckNeighboring(int x, int y, int value, int d) {
    if (x < 1 || kFieldWidth - 1 <= x || y < 0 || kFieldHeight - 1 <= y || field_check_[y][x] > 0) {
        return;
    }
    if (field_float_[y][x] == kMaxColorNum - 1) {
        // おじゃまぷよ
        field_check_[y][x] = 2;
        return;
    } else if (field_float_[y][x] != value) {
        // 検索している色と違う色だったら、訪問済みに
        field_check_[y][x] = 3;
        return;
    }

    // 同じ色だったら、削除に
    field_check_[y][x] = 1;
    for (int i = 0; i < 4; i++) {
        if (i != d) {
            RecCheckNeighboring(x + kDxs[i], y + kDys[i], value, kDds[i]);
        }
    }
}

void Game::Blink() {
    for (int i = 0; i < kBlinkNum; i++) {
        Show();
        delay(kBlinkClockMillis);

        for (int y = 0; y < kHeight; y++) {
            for (int x = 0; x < kWidth; x++) {
                LED::SetColor(x, y, kPuyoColors[field_fixed_[y + 1][x + 1]]);
            }
        }
        LED::Update();
        delay(kBlinkClockMillis);
    }
}

void Game::Show() {
    for (int y = 0; y < kHeight; y++) {
        for (int x = 0; x < kWidth; x++) {
            LED::SetColor(x, y, kPuyoColors[field_float_[y + 1][x + 1]]);
        }
    }

    LED::SetColor(LED::kLEDNum - 4, kPuyoColors[next_puyos[0]]);
    LED::SetColor(LED::kLEDNum - 3, kPuyoColors[next_puyos[1]]);
    LED::SetColor(LED::kLEDNum - 2, kPuyoColors[next2_puyos[0]]);
    LED::SetColor(LED::kLEDNum - 1, kPuyoColors[next2_puyos[1]]);

    LED::Update();
}

void Game::Stop() {
    unsigned long time_millis = millis();
    unsigned long diff_next_clock_millis = (next_clock_millis_ > time_millis) ? next_clock_millis_ - time_millis : 0;
    unsigned long diff_next_input_clock_millis = (next_input_clock_millis_ > time_millis) ? next_input_clock_millis_ - time_millis : input_clock_cycle_millis_;

    // ボタンを離すまで無限ループ
    while (p_nintendo_->buttons() & SNES_START);
    // ボタンが押されるまで無限ループ
    while (!(p_nintendo_->buttons() & SNES_START));
    // ボタンを離すまで無限ループ
    while (p_nintendo_->buttons() & SNES_START);

    time_millis = millis();
    next_clock_millis_ = time_millis + diff_next_clock_millis;
    next_input_clock_millis_ = time_millis + diff_next_input_clock_millis;
}

void Game::Reset() {
    // 色有りぷよを白に
    for (int y = kHeight - 1; y >= 0; y--) {
        for (int x = 0; x < kWidth; x++) {
            int xx = (y % 2) ? x : kWidth - 1 - x;
            if (field_float_[y + 1][xx + 1]) {
                LED::SetColor(xx, y, WHITE);
                LED::Update();
                delay(kEffectClockMillis);
            }
        }
    }
    for (int i = LED::kLEDNum - 4; i < LED::kLEDNum; i++) {
        LED::SetColor(i, WHITE);
        LED::Update();
        delay(kEffectClockMillis);
    }

    // 全面白に
    for (int y = kHeight - 1; y >= 0; y--) {
        for (int x = 0; x < kWidth; x++) {
            int xx = (y % 2) ? x : kWidth - 1 - x;
            if (!field_float_[y + 1][xx + 1]) {
                LED::SetColor(xx, y, WHITE);
                LED::Update();
                delay(kEffectClockMillis);
            }
        }
    }

    // 全面クリア
    for (int y = kHeight - 1; y >= 0; y--) {
        for (int x = 0; x < kWidth; x++) {
            int xx = (y % 2) ? x : kWidth - 1 - x;
            LED::SetColor(xx, y, CLEAR);
            LED::Update();
            delay(kEffectClockMillis);
        }
    }
    for (int i = LED::kLEDNum - 4; i < LED::kLEDNum; i++) {
        LED::SetColor(i, CLEAR);
        LED::Update();
        delay(kEffectClockMillis);
    }
}

void Game::Over() {
    // ブリンク
    memcpy(field_float_, field_fixed_, sizeof(field_float_));
    for (int i = 0; i < kGameOverBlinkNum; i++) {
        LED::ClearAll();
        LED::Update();
        delay(kGameOverBlinkClockMillis);
        Show();
        delay(kGameOverBlinkClockMillis);
    }
    Reset();
}

void Game::Init() {
    is_over_ = false;
    jammer_puyo_counter_ = jammer_puyo_cycle_;

    memset(field_fixed_, 0, sizeof(field_fixed_));
    memset(field_float_, 0, sizeof(field_float_));

    for (int y = 0; y < kFieldHeight; y++) {
        field_fixed_[y][0] = field_fixed_[y][kFieldWidth - 1] = kMaxColorNum;
        field_float_[y][0] = field_float_[y][kFieldWidth - 1] = kMaxColorNum;
    }
    for (int x = 0; x < kFieldWidth; x++) {
        field_fixed_[kFieldHeight - 1][x] = field_float_[kFieldHeight - 1][x] = kMaxColorNum;
    }

    CreatePuyo(next_puyos);
    CreatePuyo(next2_puyos);
    SetPuyo();

    next_clock_millis_ = millis() + clock_cycle_millis_;
    next_input_clock_millis_ = 0;
}

void Game::Start() {
    Init();

    while (!is_over_) {
        // 描画
        Show();

        // 入力
        if (int input = p_nintendo_->buttons()) {
            // リセット
            if ((input & kResetInput) == kResetInput) {
                Reset();
                return;
            }
            // ストップ
            if (input & SNES_START) {
                Stop();
                continue;
            }

            unsigned long input_time_millis = millis();
            if (input_time_millis > next_input_clock_millis_) {
                ControlPuyo(input);
                next_input_clock_millis_ = millis();
                next_input_clock_millis_ += (input & SNES_DOWN) ? down_input_clock_cycle_millis_ : input_clock_cycle_millis_;
            }
        } else {
            // 押しっぱなし解除時にはすぐに入力を受け付ける
            next_input_clock_millis_ = 0;
        }

        // 一定時間たったとき
        unsigned long time_millis = millis();
        if (time_millis > next_clock_millis_) {
            // まだ落ちるとき
            if (!CheckOverlap(0, 1)) {
                // 1マス下に移動
                MovePuyo(0, 1);
            } else {
                // 固定
                memcpy(field_fixed_, field_float_, sizeof(field_fixed_));
                // 浮いてるぷよを落とす
                FallPuyo();
                // 連鎖と移動
                while (SearchAndDeletePuyo()) {
                    Blink();
                    FallPuyo();
                }
                // おじゃまぷよを配置
                if (--jammer_puyo_counter_ < 0) {
                    SetJammerPuyo();
                    jammer_puyo_counter_ = jammer_puyo_cycle_;
                }
                // 新しいぷよを配置
                SetPuyo();
            }
            // 次クロック確定
            next_clock_millis_ = millis() + clock_cycle_millis_;
        }
    }
    Over();
}
