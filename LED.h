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
    const static int kRedValue   = 100;
    const static int kGreenValue = 100;
    const static int kBlueValue  = 100;

    void SetColorNC(int index, COLOR c);

public:
    const static int kLEDNum = 76;
    const static int kWidth  = 6;
    const static int kHeight = 12;

    LED();

    void Setup();
    void Update();
    void ClearAll();
    void SetColor(int index, COLOR c);
    void SetColor(int x, int y, COLOR c);
    void SetAllColor(COLOR c);
};

#endif /* _LED_H_ */
