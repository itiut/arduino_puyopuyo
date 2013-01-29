#include <string.h>
#include <Tlc5940.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"
#include "graphic.h"

SNESpad nintendo = SNESpad(6, 5, 7);
Game game = Game(&nintendo);
long rgbs[LED::kHeight][LED::kWidth];
unsigned long next_clock_millis;
unsigned long clock_cycle_millis = 20;

void setup() {
  randomSeed(analogRead(0));
  LED::Setup();

  memset(rgbs, 0, sizeof(rgbs));
  next_clock_millis = millis() + clock_cycle_millis;
}

void loop() {
  if (nintendo.buttons() & SNES_START) {
    /* スタートボタンを離したらゲームスタート */
    while (nintendo.buttons() & SNES_START);

    Graphic::CountDown();
    game.Start();

    memset(rgbs, 0, sizeof(rgbs));
    next_clock_millis = millis() + clock_cycle_millis;
  }

  unsigned long time_millis = millis();
  if (time_millis > next_clock_millis) {
    int x = random(0, LED::kWidth);
    int y = random(0, LED::kHeight);
    long r = random(0, 256);
    long g = random(0, 256);
    long b = random(0, 256);
    rgbs[y][x] = (r << 16) | (g << 8) | b;
    next_clock_millis = millis() + clock_cycle_millis;
  }

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      long r = (rgbs[y][x] >> 16) & 0xff;
      long g = (rgbs[y][x] >> 8) & 0xff;
      long b = rgbs[y][x] & 0xff;
      LED::SetAnalogColor(x, y, r, g, b);
      rgbs[y][x] = (max(0, r - 1) << 16) | (max(0, g - 1) << 8) | max(0, b - 1);
    }
  }
  LED::Update();
}