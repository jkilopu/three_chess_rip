#ifndef _MAP_H
#define _MAP_H

#include "map_def.h"

Map *create_empty_map(unsigned int h, unsigned int w);
Map *create_copy_of_map(Map *src_map);
void init_map(Map *map, Player *players, PlayerIdx player_num, Path paths[], unsigned int path_num);
void print_map(const Map *map);
void destroy_map(Map *map);

#endif
