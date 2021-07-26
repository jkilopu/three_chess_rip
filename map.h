#ifndef _MAP_H
#define _MAP_H

#include "map_def.h"
#include "player_array_def.h"


#define inner_val(_v) ((_v) * 2 + 1)
#define visable_val(_v) (((_v) - 1) / 2)

void create_empty_local_map(Map *map, unsigned int h, unsigned int w);
void clear_map(Map *map);
void create_copy_of_local_map(Map *new_map, const Map *src_map);
void copy_map(Map *dst_map, const Map *src_map);
void copy_map_element(Map *dst_map, const Map *src_map);
void init_map(Map *map, PlayerArray *player_array, const Path paths[], unsigned int path_num);
void print_map(const Map *map);
void destroy_local_map(Map *map);
void destroy_map(Map *map);

#endif
