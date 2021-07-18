#ifndef _GAME_DEF_H
#define _GAME_DEF_H

#include "map_def.h"
#include <stdbool.h>

typedef struct {
    Map *map;
    char status[PLAYER_NUM];
    Player round_player;
    Player user_player;
    Player out_player;
    bool out_chess_moved;
    unsigned int round_num;
    Point2D chess_records[PLAYER_NUM][CHESS_PER_PLAYER];
} Game;

#endif
