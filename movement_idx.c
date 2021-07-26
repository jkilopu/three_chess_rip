#include "movement_idx.h"

MovementIdx idx_of_movement(Point2D pos, DirectionIdx dir, unsigned int map_visable_h, unsigned int map_visable_w)
{
    return dir * map_visable_h * map_visable_w + pos.y * map_visable_w + pos.x;
}

DirectionIdx idx_to_movement(MovementIdx movement_idx, Point2D *p_pos, unsigned int map_visable_h, unsigned int map_visable_w)
{
    p_pos->x = movement_idx % map_visable_w;
    p_pos->y = (movement_idx / map_visable_w) % map_visable_h;
    return (DirectionIdx) movement_idx / (map_visable_h * map_visable_w);
}

