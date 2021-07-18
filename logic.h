#ifndef _LOGIC_H
#define _LOGIC_H

#include "game_def.h"
#include "map_def.h"
#include <stdbool.h>

#define DIRECTION_NUM (DOWN_RIGHT - UP + 1)
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

#define LOSE_BIT 0
#define OUT_BIT 1
#define is_lose(status) (status & (1 << LOSE_BIT))
#define set_lose(status) (status |= (1 << LOSE_BIT))
#define set_not_lose(status) (status &= ~(1 << LOSE_BIT))
#define is_out(status) (status & (1 << OUT_BIT))
#define set_out(status) (status |= (1 << OUT_BIT))

bool move(Game *game, Point2D pos, Direction dir);
unsigned int get_available_move_choice(Game *game, Point2D movable_poses[], Direction movable_dirs[][DIRECTION_NUM], unsigned int dir_cnts[]);
void player_lose(Game *game);

#endif
