#ifndef _ROUND_DEF_H
#define _ROUND_DEF_H

#include "map_def.h"
#include "player_array_def.h"
#include "player_def.h"
#include <stdbool.h>

typedef struct {
    Map map;
    PlayerArray player_array;
    PlayerIdx round_player_idx;
    PlayerIdx out_player_idx;
    bool out_chess_moved;
    unsigned int round_num;
} Round;

typedef enum {
    GAME_END,
    GAME_RUN,
    MOVE_FAIL,
} RoundStatus;

typedef struct {
    RoundStatus status;
    PlayerIdx last_player_idx;
} GameEnd;

typedef union {
    RoundStatus status;
    GameEnd game_end;
} RoundInfo;

#endif
