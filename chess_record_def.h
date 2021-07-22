#ifndef _CHESS_RECORD_DEF_H
#define _CHESS_RECORD_DEF_H

#include "player_def.h"
#include "chess_def.h"

typedef struct {
    PlayerIdx p_idx;
    ChessIdx c_idx;
} ChessRecord;

#endif
