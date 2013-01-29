#ifndef _GAME_H_
#define _GAME_H_

#include <SNESpad.h>
#include "LED.h"

class Game {
private:
    static const int kWidth = 6;
    static const int kHeight = 12;
    static const int kFieldWidth = kWidth + 2;
    static const int kFieldHeight = kHeight + 2;
    static const int kPuyoWidth = 3;
    static const int kPuyoHeight = 3;
    static const int kColorNum = 5;
    static const int kMaxColorNum = 8;
    static const COLOR kPuyoColors[kMaxColorNum];
    static const int kDxs[4];
    static const int kDys[4];
    static const int kDds[4];
    static const int kDeletePuyoNum = 4; /* 消去に必要な個数 */
    static const int kFallClockMillis = 100;
    static const int kBlinkNum = 4;
    static const int kBlinkClockMillis = 100;
    static const int kGameOverBlinkNum = 5;
    static const int kGameOverBlinkClockMillis = 400;
    static const int kResetInput = 0xc0c; /* START, SELECT, L, R */

    int field_fixed_[kFieldHeight][kFieldWidth]; /* 固定 */
    int field_float_[kFieldHeight][kFieldWidth]; /* 固定+浮遊 (表示用) */

    int field_check_[kFieldHeight][kFieldWidth]; /* チェック用 */
    /* 0 -> 未訪問, 1 -> 削除, 2 -> おじゃま削除, 3 -> 訪問済み */

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
    int jammer_puyo_counter_;
    int jammer_puyo_cycle_;
    unsigned long clock_cycle_millis_;
    unsigned long next_clock_millis_;
    unsigned long input_clock_cycle_millis_;
    unsigned long down_input_clock_cycle_millis_;
    unsigned long next_input_clock_millis_;

    void CreatePuyo(int *puyos);
    void SetPuyo();
    void SetJammerPuyo();
    bool CheckOverlap(int dx, int dy);
    void MovePuyo(int dx, int dy);
    void TurnPuyo(bool clockwise);
    void ControlPuyo(int input);
    void FallPuyo();
    bool DownOneRow();
    bool SearchAndDeletePuyo();
    bool DeletePuyo(int cx, int cy);
    void RecCheckNeighboring(int x, int y, int value, int d);
    void Blink();
    void Show();
    void Init();

public:
    Game(SNESpad *p_nintendo, LED *p_led);
    virtual ~Game();

    void Start();
};

#endif /* _GAME_H_ */
