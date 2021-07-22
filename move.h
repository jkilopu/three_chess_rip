#ifndef _LOGIC_H
#define _LOGIC_H

#include "move_def.h"
#include "game_def.h"
#include "point_def.h"
#include "direction_def.h"

void handle_move_info(MoveInfo move_info);
bool move(Round *round, Point2D pos, Direction dir);
unsigned int get_available_move_choice(Round *round, Point2D movable_poses[], Direction movable_dirs[][DIRECTION_NUM], unsigned int dir_cnts[]);
PlayerIdx player_out(Round *round);
void print_move_choice(Point2D pos, Direction dir);

#endif
