#include <SNESpad.h>
#include "LED.h"
#include "game.h"
#include "graphic.h"

SNESpad nintendo = SNESpad(6, 5, 7);
Game game = Game(&nintendo);

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);

  LED::Setup();

  LED::SetAllColor(RED);
  LED::Update();
}

void loop() {
  if (nintendo.buttons() & SNES_START) {
    /* スタートボタンを離したらゲームスタート */
    while (nintendo.buttons() & SNES_START);

    game.Start();
    delay(1000);
    LED::SetAllColor(GREEN);
    LED::Update();
  }
  delay(250);
}
