#ifndef _DIRECTION_DEF_H
#define _DIRECTION_DEF_H

#define DIRECTION_NUM (DOWN_RIGHT - UP + 1)

typedef unsigned char DirectionIdx;
typedef enum {
    INVALID_DIRECTION,
    UP,
    DOWN,
    LEFT,
    RIGHT,
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
} Direction;

#endif
