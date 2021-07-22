#ifndef _MOVE_DEF_H
#define _MOVE_DEF_H

typedef enum {
    UNKNOWN_OPERATION,
    MOVE_SUCCESS,
    POS_OUT_OF_BOUND,
    WRONG_PLAYER,
    DOUBLE_CHESS_MOVED_LAST_TIME,
    NOT_DOUBLE_CHESS,
    HAS_CHESS,
    UP_HAS_OBSTACLE,
    DOWN_HAS_OBSTACLE,
    LEFT_HAS_OBSTACLE,
    RIGHT_HAS_OBSTACLE,
    UP_LEFT_HAS_OBSTACLE,
    UP_RIGHT_HAS_OBSTACLE,
    DOWN_LEFT_HAS_OBSTACLE,
    DOWN_RIGHT_HAS_OBSTACLE,
} MoveInfo;

#endif