#include <Tlc5940.h>
#include "LED.h"

void LED::Init() {
    Tlc.init();
}

void LED::ClearAll() {
    Tlc.clear();
}

void LED::Update() {
    Tlc.update();
}

void LED::SetColorNC(int index, COLOR c) {
    int channel = index * 3;
    Tlc.set(channel, (c & RED) ? LED::kRedValue : 0);
    Tlc.set(channel + 1, (c & GREEN) ? LED::kGreenValue : 0);
    Tlc.set(channel + 2, (c & BLUE) ? LED::kBlueValue : 0);
}

void LED::SetColor(int index, COLOR c) {
    if (index < 0 || LED::kLEDNum <= index) {
        return;
    }
    SetColorNC(index, c);
}

void LED::SetColor(int x, int y, COLOR c) {
    if (x < 0 || LED::kWidth <= x || y < 0 || LED::kHeight <= y) {
        return;
    }
    SetColorNC(y * LED::kWidth + x, c);
}
