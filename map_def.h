#ifndef _MAP_DEF_H
#define _MAP_DEF_H

#include "chess_record_def.h"
#include "road_def.h"
#include "point_def.h"

typedef union {
    ChessRecord chess_record;
    Road road;
} MapElem;

typedef struct {
    MapElem **m;
    unsigned int inner_h, inner_w;
    unsigned int visable_h, visable_w;
} Map;

typedef PairPoint2D Path;

#endif
