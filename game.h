#ifndef _GAME_H_
#define _GAME_H_

#include <SNESpad.h>
#include "LED.h"

class Game {
private:
    static const unsigned char kWidth = 6;
    static const unsigned char kHeight = 12;
    static const unsigned char kVanish = 4; /* 消去に必要な個数 */
    static const unsigned char kColorNum = 8; /* 色数 */
    static const unsigned char kFieldWidth = kWidth + 2;
    static const unsigned char kFieldHeight = kHeight + 1;
    static const unsigned char kPuyoWidth = 3;
    static const unsigned char kPuyoHeight = 3;
    static const COLOR kPuyoColors[kColorNum];

    unsigned char field_fixed_[kFieldHeight][kFieldWidth]; /* 固定 */
    unsigned char field_float_[kFieldHeight][kFieldWidth]; /* 固定+浮遊 */
    unsigned char field_fixed_vanish_[kFieldHeight][kFieldWidth];
    unsigned char field_float_vanish_[kFieldHeight][kFieldWidth];

    struct Puyo {
        unsigned char x_;
        unsigned char y_;
        unsigned char field_[kPuyoWidth][kPuyoHeight];
    };

    Puyo puyo_;
    Puyo puyo_next_;
    Puyo puyo_next2_;

    SNESpad *p_nintendo_;
    LED *p_led_;

    bool is_over_;
    unsigned long clock_cycle_millis_;
    unsigned long next_clock_millis_;

    void CreatePuyo(Puyo *p_puyo);
    void SetNextPuyo();
    bool CheckOverlap(Puyo *p_puyo, unsigned char dx, unsigned char dy);
    void MovePuyo(Puyo *p_puyo, unsigned char dx, unsigned char dy);
    void Init();
    void Show();

public:
    Game(SNESpad *p_nintendo, LED *p_led);
    virtual ~Game();

    void Start();
};

#endif /* _GAME_H_ */
