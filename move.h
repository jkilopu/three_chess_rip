#ifndef _MOVE_H
#define _MOVE_H

#include "move_def.h"
#include "game_def.h"
#include "point_def.h"
#include "direction_def.h"

MoveInfo move_try(Round *round, Point2D pos, DirectionIdx dir, bool move_on_success);
bool move(Round *round, Point2D pos, DirectionIdx dir);
ChessIdx get_available_move_choice(Round *round, Point2D movable_poses[], DirectionIdx movable_dirs[][DIRECTION_NUM], DirectionIdx dir_cnts[]);
PlayerIdx player_out(Round *round);
void handle_move_info(MoveInfo move_info);
void print_move_choice(Point2D pos, DirectionIdx dir);

#endif
