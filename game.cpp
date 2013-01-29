#include <string.h>
#include <Arduino.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"

const COLOR Game::kPuyoColors[kColorNum] = {
    CLEAR, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE,
};

Game::Game(SNESpad *p_nintendo, LED *p_led) {
    p_nintendo_ = p_nintendo;
    p_led_ = p_led;

    clock_cycle_millis_ = 1000;
    input_clock_cycle_millis_ = 250;
}

Game::~Game() {
}

void Game::CreatePuyo(int *puyos) {
    puyos[0] = random(1, kColorNum - 1);
    puyos[1] = random(1, kColorNum - 1);
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

void Game::LockPuyo() {
    memcpy(field_fixed_, field_float_, sizeof(field_fixed_));
}

void Game::FallPuyo() {
    while (SlideOneRaw()) {
        delay(kFallDelayMillis);
        Show();
    }
    memcpy(field_fixed_, field_float_, sizeof(field_fixed_));
}

bool Game::SlideOneRaw() {
    bool slide = false;
    for (int y = kFieldHeight - 2; y > 0; y--) {
        for (int x = 1; x < kFieldWidth - 1; x++) {
            if (field_float_[y][x] == 0 && field_float_[y - 1][x] != 0) {
                field_float_[y][x] = field_float_[y - 1][x];
                field_float_[y - 1][x] = 0;
                // 必要?
                field_float_vanish_[y][x] = field_float_vanish_[y - 1][x];
                field_float_vanish_[y - 1][x] = 0;
                slide = true;
            }
        }
    }
    return slide;
}

void Game::Init() {
    is_over_ = false;

    memset(field_fixed_, 0, sizeof(field_fixed_));
    memset(field_float_, 0, sizeof(field_float_));
    memset(field_fixed_vanish_, 0, sizeof(field_fixed_vanish_));
    memset(field_float_vanish_, 0, sizeof(field_float_vanish_));

    // TODO: マジックナンバー9をどうにかする
    for (int y = 0; y < kFieldHeight; y++) {
        field_fixed_[y][0] = field_fixed_[y][kFieldWidth - 1] = 9;
        field_float_[y][0] = field_float_[y][kFieldWidth - 1] = 9;
    }
    for (int x = 0; x < kFieldWidth; x++) {
        field_fixed_[kFieldHeight - 1][x] = field_float_[kFieldHeight - 1][x] = 9;
    }

    CreatePuyo(next_puyos);
    CreatePuyo(next2_puyos);
    SetPuyo();

    next_clock_millis_ = millis() + clock_cycle_millis_;
    next_input_clock_millis_ = 0;
}

void Game::Show() {
    for (int y = 0; y < kHeight; y++) {
        for (int x = 0; x < kWidth; x++) {
            p_led_->SetColor(x, y, kPuyoColors[field_float_[y + 1][x + 1]]);
        }
    }

    // TODO: マジックナンバーなんとかする
    p_led_->SetColor(72, kPuyoColors[next_puyos[0]]);
    p_led_->SetColor(73, kPuyoColors[next_puyos[1]]);
    p_led_->SetColor(74, kPuyoColors[next2_puyos[0]]);
    p_led_->SetColor(75, kPuyoColors[next2_puyos[1]]);

    p_led_->Update();
}

void Game::Start() {
    Init();

    while (!is_over_) {
        // 描画
        Show();

        // 入力
        if (int input = p_nintendo_->buttons()) {
            unsigned long input_time_millis = millis();
            if (input_time_millis > next_input_clock_millis_) {
                ControlPuyo(input);
                // TODO: Down押しっぱなし時のクロックを短めに
                next_input_clock_millis_ = millis() + input_clock_cycle_millis_;
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
                LockPuyo();
                // 浮いてるぷよを落とす
                FallPuyo();
                // 連鎖と移動

                // 新しいぷよを配置
                SetPuyo();
            }
            // 次クロック確定
            next_clock_millis_ = millis() + clock_cycle_millis_;
        }
    }
}
