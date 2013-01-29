#include <Arduino.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"

Game::Game(SNESpad *p_nintendo, LED *p_led) {
    is_over_ = false;
    p_nintendo_ = p_nintendo;
    p_led_ = p_led;
}

Game::~Game() {
}

void Game::Start() {
    while (!is_over_) {
        int state = p_nintendo_->buttons();

        if (state & SNES_LEFT){
            p_led_->SetAllColor(GREEN);
        }
        if (state & SNES_RIGHT){
            p_led_->SetAllColor(RED);
        }
        if (state & SNES_DOWN){
            p_led_->SetAllColor(BLUE);
        }
        if (state & SNES_A){
            p_led_->SetAllColor(CYAN);
        }
        if (state & SNES_B){
            p_led_->SetAllColor(MAGENTA);
        }
        if (state & SNES_START) {
            p_led_->SetAllColor(YELLOW);
        }
        if (state & SNES_SELECT){
            p_led_->SetAllColor(WHITE);
        }
        if (!state) {
            p_led_->ClearAll();
        }
        p_led_->Update();
    }
}
