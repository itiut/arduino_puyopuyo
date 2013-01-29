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
    static const int kRedValue   = 100;
    static const int kGreenValue = 100;
    static const int kBlueValue  = 100;

    void SetColorNC(int index, COLOR c);

public:
    static const int kLEDNum = 76;
    static const int kWidth  = 6;
    static const int kHeight = 12;

    LED();

    void Setup();
    void Update();
    void ClearAll();
    void SetColor(int index, COLOR c);
    void SetColor(int x, int y, COLOR c);
    void SetAllColor(COLOR c);
};

#endif /* _LED_H_ */
