#include <Arduino.h>
#include <Tlc5940.h>
#include "LED.h"

LED::LED() {
}

void LED::Setup() {
    // Tlc.init()はsetup()の中でないと動かない?
    Tlc.init();
}

void LED::Update() {
    Tlc.update();
}

void LED::ClearAll() {
    Tlc.clear();
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
    // 配線ミスにより、次ぷよ組みと次々ぷよ組の順番が逆になったので修正
    if (index == LED::kLEDNum - 4 || index == LED::kLEDNum - 3) {
        index += 2;
    } else if (index == LED::kLEDNum - 2 || index == LED::kLEDNum - 1) {
        index -= 2;
    }
    LED::SetColorNC(index, c);
}

void LED::SetColor(int x, int y, COLOR c) {
    if (x < 0 || LED::kWidth <= x || y < 0 || LED::kHeight <= y) {
        return;
    }
    LED::SetColorNC(y * LED::kWidth + x, c);
}

void LED::SetAllColor(COLOR c) {
    for (int i = 0; i < LED::kLEDNum; i++) {
        LED::SetColorNC(i, c);
    }
}

void LED::SetAnalogColor(int x, int y, int r, int g, int b) {
    if (x < 0 || LED::kWidth <= x || y < 0 || LED::kHeight <= y) {
        return;
    }
    int channel = (y * LED::kWidth + x) * 3;
    Tlc.set(channel, constrain(r, 0, LED::kMaxValue));
    Tlc.set(channel + 1, constrain(g, 0, LED::kMaxValue));
    Tlc.set(channel + 2, constrain(b, 0, LED::kMaxValue));
}
