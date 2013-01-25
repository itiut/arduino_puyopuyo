#include <Tlc5940.h>
#include "LED.h"

LED::LED() {
    Tlc.init();
}

LED::~LED() {
}


void LED::ClearAll() {
    Tlc.clear();
}

void LED::Update() {
    Tlc.update();
}

void LED::SetColor(int index, LED::COLOR c) {
    index *= 3;
    Tlc.set(index, (c & LED::RED) ? LED::kRedValue : 0);
    Tlc.set(index + 1, (c & LED::GREEN) ? LED::kGreenValue : 0);
    Tlc.set(index + 2, (c & LED::BLUE) ? LED::kBlueValue : 0);
}

void LED::SetColor(int x, int y, COLOR c) {
    if (0 <= x && x < LED::kWidth && 0 <= y && y < LED::kHeight) {
        LED::SetColor(y * LED::kWidth + x, c);
    }
}
