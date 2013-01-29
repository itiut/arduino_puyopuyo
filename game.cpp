#include <string.h>
#include <Arduino.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"

Game::Game(SNESpad *p_nintendo, LED *p_led) {
    p_nintendo_ = p_nintendo;
    p_led_ = p_led;

    clock_cycle_millis_ = 1000;
}

Game::~Game() {
}

void Game::CreatePuyo(Puyo *p_puyo) {
    p_puyo->x_ = p_puyo->y_ = 0;
    memset(&p_puyo->field_, 0, sizeof(p_puyo->field_));
    p_puyo->field_[0][1] = random(kColorNum);
    p_puyo->field_[1][1] = random(kColorNum);
}


void Game::SetNextPuyo() {
    memcpy(&puyo_, &puyo_next_, sizeof(Puyo));
    memcpy(&puyo_next_, &puyo_next2_, sizeof(Puyo));
    CreatePuyo(&puyo_next2_);

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

bool Game::CheckOverlap(Puyo *p_puyo, unsigned char dx, unsigned char dy) {
    unsigned char nx = p_puyo->x_ + dx;
    unsigned char ny = p_puyo->y_ + dy;
    for (unsigned char y = 0; y < kPuyoHeight; y++) {
        for (unsigned char x = 0; x < kPuyoWidth; x++) {
            if (p_puyo->field_[y][x] > 0 && field_fixed_[ny + y][nx + x] != 0) {
                return true;
            }
        }
    }
    return false;
}

void Game::MovePuyo(Puyo *p_puyo, unsigned char dx, unsigned char dy) {
    memcpy(field_float_, field_fixed_, sizeof(field_float_));

    p_puyo->x_ += dx;
    p_puyo->y_ += dy;

    for (unsigned char y = 0; y < kPuyoHeight; y++) {
        for (unsigned char x = 0; x < kPuyoWidth; x++) {
            field_float_[p_puyo->y_ + y][p_puyo->x_ + x] += p_puyo->field_[y][x];
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

    CreatePuyo(&puyo_next_);
    CreatePuyo(&puyo_next2_);
    SetNextPuyo();

    Show();
    next_clock_millis_ = millis() + clock_cycle_millis_;
}

COLOR Game::PuyoColor(unsigned char value) {
    switch (value) {
    case 1:  return RED;
    case 2:  return GREEN;
    case 3:  return BLUE;
    case 4:  return CYAN;
    case 5:  return MAGENTA;
    case 6:  return YELLOW;
    case 7:  return WHITE;
    default: return CLEAR;
    }
}

void Game::Show() {
    for (unsigned char y = 0; y < kHeight; y++) {
        for (unsigned char x = 0; x < kWidth; x++) {
            p_led_->SetColor(x, y, PuyoColor(field_float_[y][x+1]));
        }
    }

    // TODO: マジックナンバーなんとかする
    p_led_->SetColor(72, PuyoColor(puyo_next_.field_[0][1]));
    p_led_->SetColor(73, PuyoColor(puyo_next_.field_[1][1]));
    p_led_->SetColor(74, PuyoColor(puyo_next2_.field_[0][1]));
    p_led_->SetColor(75, PuyoColor(puyo_next2_.field_[1][1]));

    p_led_->Update();
}

void Game::Start() {
    Init();

    while (!is_over_) {
        // 入力
        if (0) {
            // 移動
        }

        // 一定時間たったとき
        unsigned long time_millis = millis();
        if (time_millis >= next_clock_millis_) {
            // まだ落ちるとき
            if (!CheckOverlap(&puyo_, 0, 1)) {
                // 一マス下に移動
                MovePuyo(&puyo_, 0, 1);
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
