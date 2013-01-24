#ifndef _LED_H_
#define _LED_H_

#include <Tlc5940.h>

class LED {
private:
    const static int kWidth      = 6;
    const static int kHeight     = 12;
    const static int kRedValue   = 1000;
    const static int kGreenValue = 1000;
    const static int kBlueValue  = 1000;

public:
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

    LED();
    virtual ~LED();

    void ClearAll();
    void Update();
    void SetColor(int channel, COLOR c);
    void SetColor(int x, int y, COLOR c);
};

#endif /* _LED_H_ */
