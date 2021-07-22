#ifndef _PLAYER_DEF_H
#define _PLAYER_DEF_H

#include "chess_def.h"

typedef unsigned char PlayerIdx;
typedef struct {
    ChessIdx chess_num;
    Chess *chesses;
} Player;

#define NULL_PLAYER_IDX ((PlayerIdx) 0xff)
#define FIRST_PLAYER_IDX_CHAR ('a')

#endif
