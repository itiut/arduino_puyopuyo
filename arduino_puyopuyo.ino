#include <Tlc5940.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"

SNESpad nintendo = SNESpad(6, 5, 7);
LED led = LED();
Game game = Game(&nintendo, &led);

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(9600);

  led.Setup();

  led.SetAllColor(RED);
  led.Update();
}

void loop() {
  int input = nintendo.buttons();
  if (input & SNES_START) {
    /* スタートボタンを離したらゲームスタート */
    while (nintendo.buttons() & SNES_START);
    game.Start();
    delay(1000);
    led.SetAllColor(GREEN);
    led.Update();
  }
  delay(250);
}
