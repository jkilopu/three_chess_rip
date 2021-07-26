#ifndef _MOVEMENT_IDX_H
#define _MOVEMENT_IDX_H

#include "movement_idx_def.h"
#include "point_def.h"
#include "direction_def.h"

MovementIdx idx_of_movement(Point2D pos, DirectionIdx dir, unsigned int map_visable_h, unsigned int map_visable_w);
DirectionIdx idx_to_movement(MovementIdx movement_idx, Point2D *p_pos, unsigned int map_visable_h, unsigned int map_visable_w);

#endif
