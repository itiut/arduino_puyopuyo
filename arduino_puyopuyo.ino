#include <Tlc5940.h>
#include "LED.h"
/* #include <SNESpad.h> */
/* SNESpad nintendo = SNESpad(6, 5, 7); */
/* byte state = 0; */

LED led;

void setup() {
  led = LED();
  /* Serial.begin(9600); */
}

void loop() {
  /* state = nintendo.buttons(); */
  /* Serial.println(~state, BIN); */
  /* delay(1000); */

  /* for (int i = 0; i < 76; i++) { */
  /*   led.SetColor(i, LED::AQUA); */
  /* } */
  /* led.Update(); */
  /* delay(1000); */

  /* for (int i = 0; i < 76; i++) { */
  /*   led.SetColor(i, LED::MAGENTA); */
  /* } */
  /* led.Update(); */
  /* delay(1000); */

  /* for (int i = 0; i < 76; i++) { */
  /*   led.SetColor(i, LED::YELLOW); */
  /* } */
  /* led.Update(); */
  /* delay(1000); */

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      led.SetColor(x, y, LED::RED);
    }
  }
  led.Update();
  delay(1000);

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      led.SetColor(x, y, LED::GREEN);
    }
  }
  led.Update();
  delay(1000);

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      led.SetColor(x, y, LED::BLUE);
    }
  }
  led.Update();
  delay(1000);
}
