#include <Tlc5940.h>
#include <SNESpad.h>
#include "LED.h"
#include "game.h"

SNESpad nintendo = SNESpad(6, 5, 7);
LED led = LED();
Game game = Game(&nintendo, &led);

void setup() {
  led.Setup();
  led.SetAllColor(RED);
  led.Update();
}

void loop() {
  byte state = nintendo.buttons();
  if (state & SNES_START) {
    game.Start();
  }
  delay(250);
}
