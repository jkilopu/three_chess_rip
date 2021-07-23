#ifndef _MAP_H
#define _MAP_H

#include "map_def.h"
#include "player_array_def.h"

#define map_val(_v) ((_v) * 2 + 1)
#define real_val(_v) (((_v) - 1) / 2)
#define to_map_pos(_y, _x) ((_y = map_val(_y)), (_x = map_val(_x)))

void create_empty_local_map(Map *map, unsigned int h, unsigned int w);
void clear_map(Map *map);
Map *create_copy_of_map(Map *src_map);
void init_map(Map *map, PlayerArray *player_array, const Path paths[], unsigned int path_num);
void print_map(const Map *map);
void destroy_local_map(Map *map);

#endif
