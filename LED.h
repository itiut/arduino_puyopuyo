#ifndef _LED_H_
#define _LED_H_

#include <Tlc5940.h>

enum COLOR {
    CLEAR   = 0x0,
    BLUE    = 0x1,
    GREEN   = 0x2,
    CYAN    = 0x3,
    RED     = 0x4,
    MAGENTA = 0x5,
    YELLOW  = 0x6,
    WHITE   = 0x7,
};

class LED {
private:
    static const int kMaxValue = 4095;
    static const int kRedValue   = 200;
    static const int kGreenValue = 300;
    static const int kBlueValue  = 300;

    LED();
    static void SetColorNC(int index, COLOR c);

public:
    static const int kLEDNum = 76;
    static const int kWidth  = 6;
    static const int kHeight = 12;

    static void Setup();
    static void Update();
    static void ClearAll();
    static void SetColor(int index, COLOR c);
    static void SetColor(int x, int y, COLOR c);
    static void SetAllColor(COLOR c);
    static void SetAnalogColor(int x, int y, int r, int g, int b);
};

#endif /* _LED_H_ */
