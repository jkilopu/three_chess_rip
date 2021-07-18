#ifndef _MAP_DEF_H
#define _MAP_DEF_H

/* Road */
#define NULL_ROAD ((Road) 0)

#define PATH_BIT ((Road) 0)
#define GO_UP_RIGHT_OR_DOWN_LEFT_BIT ((Road) 1)
#define GO_UP_LEFT_OR_DOWN_RIGHT_BIT ((Road) 2)

#define IS_PATH(road) ((1 << PATH_BIT) & road)
#define SET_PATH(road) (road |= (1 << PATH_BIT))

#define CAN_GO_UP_RIGHT_OR_DOWN_LEFT(road) ((1 << GO_UP_RIGHT_OR_DOWN_LEFT_BIT) & road)
#define SET_GO_UP_RIGHT_OR_DOWN_LEFT(road) (road |= (1 << GO_UP_RIGHT_OR_DOWN_LEFT_BIT))

#define CAN_GO_UP_LEFT_OR_DOWN_RIGHT(road) ((1 << GO_UP_LEFT_OR_DOWN_RIGHT_BIT) & road)
#define SET_GO_UP_LEFT_OR_DOWN_RIGHT(road) (road |= (1 << GO_UP_LEFT_OR_DOWN_RIGHT_BIT))

/* Player */
#define PLAYER_NUM 3
#define CHESS_PER_PLAYER 3
#define FIRST_PLAYER ((Player ) 1)
#define LAST_PLAYER (FIRST_PLAYER + PLAYER_NUM - 1)
#define FIRST_PLAYER_CHAR ('a')
#define PLAYER_IDX(player) (player - FIRST_PLAYER)
#define NULL_PLAYER ((Player) 0)
#define PLAYER_TO_CHAR(player) (FIRST_PLAYER_CHAR + PLAYER_IDX(player)) // 如果char是有符号的会有问题吗

typedef unsigned char Road;
typedef unsigned char Player; // 作为Index使用是不是不太好
typedef unsigned char ChessIdx;
typedef struct {
    Player player;
    ChessIdx idx;
} Chess;
typedef union {
    Chess chess;
    Road road;
} MapElem;

typedef struct {
    MapElem **m;
    unsigned int w, h;
} Map;

typedef struct {
    unsigned int y, x;
} Point2D;
typedef struct {
    Point2D first;
    Point2D second;
} PairPoint2D;
typedef PairPoint2D Path;

#endif
