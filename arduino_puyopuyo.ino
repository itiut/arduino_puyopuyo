#include <Tlc5940.h>
/* #include <SNESpad.h> */
/* SNESpad nintendo = SNESpad(6, 5, 7); */
/* byte state = 0; */

int i, j;

void setup() {
  Tlc.init();

  /* Serial.begin(9600); */
}

void loop() {
  /* state = nintendo.buttons(); */
  /* Serial.println(~state, BIN); */
  /* delay(1000); */

  for (j = 0; j < 3; j++) {
    Tlc.clear();
    for (i = j; i < NUM_TLCS * 16; i += 3) {
      Tlc.set(i, 1000);
    }
    Tlc.update();
    delay(2000);
  }
}
