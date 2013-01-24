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

void LED::SetColor(int channel, LED::COLOR c) {
    if (c & LED::RED) {
        Tlc.set(channel, LED::kRedValue);
    }
    if (c & LED::GREEN) {
        Tlc.set(channel + 1, LED::kGreenValue);
    }
    if (c & LED::BLUE) {
        Tlc.set(channel + 2, LED::kBlueValue);
    }
}

void LED::SetColor(int x, int y, COLOR c) {
    if (0 <= x && x < LED::kWidth && 0 <= y && y < LED::kHeight) {
        LED::SetColor(y * LED::kWidth + x, c);
    }
}
