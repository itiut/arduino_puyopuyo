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

void Game::CreatePuyo(unsigned char *puyos) {
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
    puyo_.x_ = 3;
    puyo_.y_ = 0;
    for (unsigned char y = 0; y < kPuyoHeight; y++) {
        for (unsigned char x = 0; x < kPuyoWidth; x++) {
            field_float_[puyo_.y_ + y][puyo_.x_ + x] += puyo_.field_[y][x];

            // 重なっていたらゲームオーバー
            if (field_float_[puyo_.y_ + y][puyo_.x_ + x] != puyo_.field_[y][x]) {
                is_over_ = true;
                return;
            }
        }
    }
}

bool Game::CheckOverlap(char dx, char dy) {
    char nx = puyo_.x_ + dx;
    char ny = puyo_.y_ + dy;
    if (nx < 0) {
        return true;
    }
    for (unsigned char y = 0; y < kPuyoHeight; y++) {
        for (unsigned char x = 0; x < kPuyoWidth; x++) {
            if (puyo_.field_[y][x] > 0 && field_fixed_[ny + y][nx + x] != 0) {
                return true;
            }
        }
    }
    return false;
}

void Game::MovePuyo(char dx, char dy) {
    memcpy(field_float_, field_fixed_, sizeof(field_float_));

    puyo_.x_ += dx;
    puyo_.y_ += dy;
    for (unsigned char y = 0; y < kPuyoHeight; y++) {
        for (unsigned char x = 0; x < kPuyoWidth; x++) {
            field_float_[puyo_.y_ + y][puyo_.x_ + x] += puyo_.field_[y][x];
        }
    }
}

void Game::TurnPuyo(bool clockwise) {
    Serial.println(clockwise);
    unsigned char temp_field[kPuyoHeight][kPuyoWidth];
    memcpy(temp_field, puyo_.field_, sizeof(temp_field));

    if (clockwise) {
        unsigned char temp_value = puyo_.field_[0][1];
        puyo_.field_[0][1] = puyo_.field_[1][0];
        puyo_.field_[1][0] = puyo_.field_[2][1];
        puyo_.field_[2][1] = puyo_.field_[1][2];
        puyo_.field_[1][2] = temp_value;
    } else {
        unsigned char temp_value = puyo_.field_[0][1];
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

void Game::Init() {
    is_over_ = false;

    memset(field_fixed_, 0, sizeof(field_fixed_));
    memset(field_float_, 0, sizeof(field_float_));
    memset(field_fixed_vanish_, 0, sizeof(field_fixed_vanish_));
    memset(field_float_vanish_, 0, sizeof(field_float_vanish_));

    // TODO: マジックナンバー9をどうにかする
    for (unsigned char y = 0; y < kFieldHeight; y++) {
        field_fixed_[y][0] = field_fixed_[y][kFieldWidth - 1] = 9;
        field_float_[y][0] = field_float_[y][kFieldWidth - 1] = 9;
    }
    for (unsigned char x = 0; x < kFieldWidth; x++) {
        field_fixed_[kFieldHeight - 1][x] = field_float_[kFieldHeight - 1][x] = 9;
    }

    CreatePuyo(next_puyos);
    CreatePuyo(next_puyos);
    SetPuyo();

    Show();
    next_clock_millis_ = millis() + clock_cycle_millis_;
    next_input_clock_millis_ = 0;
}


void Game::Show() {
    for (unsigned char y = 0; y < kHeight; y++) {
        for (unsigned char x = 0; x < kWidth; x++) {
            p_led_->SetColor(x, y, kPuyoColors[field_float_[y][x+1]]);
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
        // 入力
        if (int input = p_nintendo_->buttons()) {
            unsigned long input_time_millis = millis();
            if (input_time_millis > next_input_clock_millis_) {
                ControlPuyo(input);
                next_input_clock_millis_ = millis() + input_clock_cycle_millis_;
            }
        } else {
            next_input_clock_millis_ = 0;
        }


        // 一定時間たったとき
        unsigned long time_millis = millis();
        if (time_millis > next_clock_millis_) {
            // まだ落ちるとき
            if (!CheckOverlap(0, 1)) {
                // 一マス下に移動
                MovePuyo(0, 1);
            } else {
                is_over_ = true;
                // 固定

                // 浮いてるぷよを落とす

                // 連鎖と移動

                // 新しいぷよ生成
            }
            // 次クロック確定
            next_clock_millis_ = millis() + clock_cycle_millis_;
        }

        // 描写
        Show();
    }
}
