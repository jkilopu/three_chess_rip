#ifndef _MAP_H
#define _MAP_H

#include "map_def.h"

Map *create_empty_map(unsigned int w, unsigned int h);
Map *create_copy_of_map(Map *src_map);
void init_map(Map *map, Point2D chess_records[PLAYER_NUM][CHESS_PER_PLAYER], Path paths[], unsigned int paths_num);
void print_map(const Map *map);
void destroy_map(Map *map);

#endif
