#include <Tlc5940.h>
#include "LED.h"
/* #include <SNESpad.h> */
/* SNESpad nintendo = SNESpad(6, 5, 7); */
/* byte state = 0; */


void setup() {
  LED::Init();
  /* Serial.begin(9600); */
}

void loop() {
  /* state = nintendo.buttons(); */
  /* Serial.println(~state, BIN); */
  /* delay(1000); */

  /* for (int i = 0; i < 76; i++) { */
  /*   LED::SetColor(i, AQUA); */
  /* } */
  /* LED::Update(); */
  /* delay(1000); */

  /* for (int i = 0; i < 76; i++) { */
  /*   LED::SetColor(i, MAGENTA); */
  /* } */
  /* LED::Update(); */
  /* delay(1000); */

  /* for (int i = 0; i < 76; i++) { */
  /*   LED::SetColor(i, YELLOW); */
  /* } */
  /* LED::Update(); */
  /* delay(1000); */

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      LED::SetColor(x, y, RED);
    }
  }
  LED::Update();
  delay(1000);

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      LED::SetColor(x, y, GREEN);
    }
  }
  LED::Update();
  delay(1000);

  for (int y = 0; y < LED::kHeight; y++) {
    for (int x = 0; x < LED::kWidth; x++) {
      LED::SetColor(x, y, BLUE);
    }
  }
  LED::Update();
  delay(1000);
}
