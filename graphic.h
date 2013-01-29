#ifndef _GRAPHIC_H_
#define _GRAPHIC_H_

#include "LED.h"

class Graphic {
private:
    typedef bool led_array_t[LED::kHeight][LED::kWidth];

    static const led_array_t one;
    static const led_array_t two;
    static const led_array_t three;

    static const int kClearClockMillis = 10;
    static const int kCountDownDelayMillis = 250;
    static const int kCountDownClockMillis = 1000;

    Graphic();

    static void PrintChar(char ch, COLOR color, int delay_millis);

public:
    static void CountDown();
};

#endif /* _GRAPHIC_H_ */
