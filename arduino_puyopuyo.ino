#include <Tlc5940.h>

//#define NUM_TLCS 1

void setup() {
  Tlc.init();
}

void loop() {
  for (int i = 0; i < NUM_TLCS*16; i++) {
    Tlc.clear();
    Tlc.set(i, 1000);
    Tlc.update();
    delay(100);
  }
}
