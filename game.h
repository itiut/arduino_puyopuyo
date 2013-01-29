#ifndef _GAME_H_
#define _GAME_H_

#include <SNESpad.h>
#include "LED.h"

class Game {
private:
    bool is_over_;
    SNESpad *p_nintendo_;
    LED *p_led_;

public:
    Game(SNESpad *p_nintendo, LED *p_led);
    virtual ~Game();

    void Start();
};

#endif /* _GAME_H_ */
