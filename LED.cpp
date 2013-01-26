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

void LED::SetColorNC(int index, COLOR c) {
    int channel = index * 3;
    Tlc.set(channel, (c & LED::RED) ? LED::kRedValue : 0);
    Tlc.set(channel + 1, (c & LED::GREEN) ? LED::kGreenValue : 0);
    Tlc.set(channel + 2, (c & LED::BLUE) ? LED::kBlueValue : 0);
}

void LED::SetColor(int index, LED::COLOR c) {
    if (index < 0 || LED::kLEDNum <= index) {
        return;
    }
    LED::SetColorNC(index, c);
}

void LED::SetColor(int x, int y, COLOR c) {
    if (x < 0 || LED::kWidth <= x || y < 0 || LED::kHeight <= h) {
        return;
    }
    LED::SetColorNC(y * LED::kWidth + x, c);
}
