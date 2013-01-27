#ifndef _GAME_H_
#define _GAME_H_

class Game {
private:
    bool is_over_;

public:
    Game();
    virtual ~Game();

    void Start();
};

#endif /* _GAME_H_ */
