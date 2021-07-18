#ifndef _ROBOT_H
#define _ROBOT_H

#include "map_def.h"
#include "error_def.h"
#include "logic.h"

RobotError robot_make_random_choice(Point2D *p_pos, Direction *p_dir);
RobotError robot_make_unique_choice(Point2D *p_pos, Direction *p_dir);
RobotError robot_make_smart_choice(Point2D *p_pos, Direction *p_dir);

#endif
