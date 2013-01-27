#ifndef _LED_H_
#define _LED_H_

#include <Tlc5940.h>

enum COLOR {
    CLEAR   = 0x0,
    BLUE    = 0x1,
    GREEN   = 0x2,
    AQUA    = 0x3,
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

    LED();

    static void SetColorNC(int index, COLOR c);

public:
    const static int kLEDNum = 76;
    const static int kWidth  = 6;
    const static int kHeight = 12;

    static void Init();
    static void ClearAll();
    static void Update();
    static void SetColor(int index, COLOR c);
    static void SetColor(int x, int y, COLOR c);
    static void SetAllColor(COLOR c);
};

#endif /* _LED_H_ */
