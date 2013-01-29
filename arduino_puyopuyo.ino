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
  byte state = nintendo.buttons();
  if (state & SNES_START) {
    Serial.println(8888);
    game.Start();
    Serial.println(9999);
    delay(1000);
    led.SetAllColor(GREEN);
    led.Update();
  }
  delay(250);
}
