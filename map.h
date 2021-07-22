#ifndef _MAP_H
#define _MAP_H

#include "map_def.h"
#include "player_array_def.h"

void create_empty_local_map(Map *map, unsigned int h, unsigned int w);
void clear_map(Map *map);
Map *create_copy_of_map(Map *src_map);
void init_map(Map *map, PlayerArray *player_array, Path paths[], unsigned int path_num);
void print_map(const Map *map);
void destroy_local_map(Map *map);

#endif
