#include "game.h"
#include <Arduino.h>

Game::Game() {
    is_over_ = false;
}

Game::~Game() {
}

void Game::Start() {
    while (!is_over_) {
    }
}
