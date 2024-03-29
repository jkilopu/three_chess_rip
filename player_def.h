#ifndef _PLAYER_DEF_H
#define _PLAYER_DEF_H

#include "chess_def.h"
#include <stdbool.h>

typedef unsigned char PlayerIdx;
typedef struct {
    Chess *chesses;
    bool out;
    ChessIdx chess_num;
} Player;

#define NULL_PLAYER_IDX ((PlayerIdx) 0xff)
#define FIRST_PLAYER_IDX_CHAR ('a')

#endif
