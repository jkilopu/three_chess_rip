#ifndef _PLAYER_ARRAY_DEF_H
#define _PLAYER_ARRAY_DEF_H

#include "player_def.h"

typedef struct {
    Player *players;
    PlayerIdx player_num;
} PlayerArray;

#endif
