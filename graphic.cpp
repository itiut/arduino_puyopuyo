#include <Arduino.h>
#include "LED.h"
#include "graphic.h"

const Graphic::led_array_t Graphic::one = {
    {0,0,0,0,0,0},
    {0,0,0,1,0,0},
    {0,0,1,1,0,0},
    {0,1,0,1,0,0},
    {0,0,0,1,0,0},
    {0,0,0,1,0,0},
    {0,0,0,1,0,0},
    {0,0,0,1,0,0},
    {0,0,0,1,0,0},
    {0,0,0,1,0,0},
    {0,1,1,1,1,0},
    {0,0,0,0,0,0}
};

const Graphic::led_array_t Graphic::two = {
    {0,0,0,0,0,0},
    {0,0,1,1,0,0},
    {0,1,0,0,1,0},
    {0,1,0,0,1,0},
    {0,0,0,0,1,0},
    {0,0,0,0,1,0},
    {0,0,0,1,0,0},
    {0,0,0,1,0,0},
    {0,0,1,0,0,0},
    {0,1,0,0,0,0},
    {0,1,1,1,1,0},
    {0,0,0,0,0,0}
};

const Graphic::led_array_t Graphic::three = {
    {0,0,0,0,0,0},
    {0,0,1,1,0,0},
    {0,1,0,0,1,0},
    {0,1,0,0,1,0},
    {0,0,0,0,1,0},
    {0,0,0,0,1,0},
    {0,0,1,1,0,0},
    {0,0,0,0,1,0},
    {0,1,0,0,1,0},
    {0,1,0,0,1,0},
    {0,0,1,1,0,0},
    {0,0,0,0,0,0}
};

Graphic::Graphic() {
}

void Graphic::PrintChar(char ch, COLOR color, int delay_millis) {
    LED::ClearAll();
    for (int y = 0; y < LED::kHeight; y++) {
        for (int x = 0; x < LED::kWidth; x++) {
            switch (ch) {
            case '1':
                if (Graphic::one[y][x]) LED::SetColor(x, y ,color);
                break;
            case '2':
                if (Graphic::two[y][x]) LED::SetColor(x, y ,color);
                break;
            case '3':
                if (Graphic::three[y][x]) LED::SetColor(x, y ,color);
                break;
            default:
                break;
            }
        }
    }
    LED::Update();
    delay(delay_millis);
}

void Graphic::CountDown() {
    for (int y = LED::kHeight - 1; y >= 0; y--) {
        for (int x = 0; x < LED::kWidth; x++) {
            int xx = (y % 2) ? x : LED::kWidth - 1 - x;
            LED::SetColor(xx, y, WHITE);
            LED::Update();
            delay(Graphic::kClearClockMillis);
        }
    }
    for (int y = LED::kHeight - 1; y >= 0; y--) {
        for (int x = 0; x < LED::kWidth; x++) {
            int xx = (y % 2) ? x : LED::kWidth - 1 - x;
            LED::SetColor(xx, y, CLEAR);
            LED::Update();
            delay(Graphic::kClearClockMillis);
        }
    }
    delay(Graphic::kCountDownDelayMillis);
    Graphic::PrintChar('3', CYAN, Graphic::kCountDownClockMillis);
    Graphic::PrintChar('2', YELLOW, Graphic::kCountDownClockMillis);
    Graphic::PrintChar('1', RED, Graphic::kCountDownClockMillis);
}
