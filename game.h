#ifndef _GAME_H_
#define _GAME_H_

#include <SNESpad.h>
#include "LED.h"

class Game {
private:
    static const int kWidth = 6;
    static const int kHeight = 12;
    static const int kVanish = 4; /* 消去に必要な個数 */
    static const int kColorNum = 8; /* 色数 */
    static const int kFieldWidth = kWidth + 2;
    static const int kFieldHeight = kHeight + 2;
    static const int kPuyoWidth = 3;
    static const int kPuyoHeight = 3;
    static const COLOR kPuyoColors[kColorNum];
    static const int kFallDelayMillis = 100;

    int field_fixed_[kFieldHeight][kFieldWidth]; /* 固定 */
    int field_float_[kFieldHeight][kFieldWidth]; /* 固定+浮遊 */
    int field_fixed_vanish_[kFieldHeight][kFieldWidth];
    int field_float_vanish_[kFieldHeight][kFieldWidth];

    struct Puyo {
        int x_;
        int y_;
        int field_[kPuyoWidth][kPuyoHeight];
    };

    Puyo puyo_;
    int next_puyos[2];
    int next2_puyos[2];

    SNESpad *p_nintendo_;
    LED *p_led_;

    bool is_over_;
    unsigned long clock_cycle_millis_;
    unsigned long next_clock_millis_;
    unsigned long input_clock_cycle_millis_;
    unsigned long next_input_clock_millis_;

    void CreatePuyo(int *puyos);
    void SetPuyo();
    bool CheckOverlap(int dx, int dy);
    void MovePuyo(int dx, int dy);
    void TurnPuyo(bool clockwise);
    void ControlPuyo(int input);
    void LockPuyo();
    void FallPuyo();

    bool SlideOneRaw();

    void Init();
    void Show();

public:
    Game(SNESpad *p_nintendo, LED *p_led);
    virtual ~Game();

    void Start();
};

#endif /* _GAME_H_ */
