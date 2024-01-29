#ifndef GAME_H
#define GAME_H

#include "def.h"
#include "player.h"
#include "enemy.h"
#include "map.h"
#include "camera.h"
#include "input_handler.h"

struct Game
{
public:
    Game();

    void run();

private:
    camera c;
    map m;
    background bg;
    player p;
    inputhandler ih;
    enemy e[MAXENEMY] = {{11, 24}, {17, 24}, {16, 21}, {21, 18}};

    void init();
#ifdef __linux__
    void *keyboardCheck();
#endif
    void *Input(inputhandler *ih, player *p);
    void *updatePlayer(player *p);
    void *updateEnemy(enemy (*e)[MAXENEMY]);
    void *render(player *p, enemy (*e)[MAXENEMY], camera *c);
    void print_data(player *p, enemy (*e)[MAXENEMY]);
    void *timer(player *p);
    void end();
    void win();
    void lose();
};

#endif // GAME_H
