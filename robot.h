#ifndef _ROBOT_H
#define _ROBOT_H

#include "robot_def.h"
#include "game_def.h"
#include "point_def.h"
#include "direction_def.h"

RobotInfo robot_make_random_choice(Game *game, Point2D *p_pos, DirectionIdx *p_dir);
RobotInfo robot_make_best_choice(Game *game, Point2D *p_pos, DirectionIdx *p_dir);
void handle_robot_info(RobotInfo robot_info);

#endif
