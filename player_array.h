#ifndef _PLAYER_ARRAY_H
#define _PLAYER_ARRAY_H

#include "player_array_def.h"
#include "player_def.h"
#include "point_def.h"

void create_empty_local_player_array(PlayerArray *player_array, const ChessIdx chess_nums[], PlayerIdx player_num);
void clear_player_array(PlayerArray *player_array);
void init_player_array(PlayerArray *player_array, const Point2D poses[]);
void create_copy_of_local_player_array(PlayerArray *new_player_array, const PlayerArray *src_player_array);
void copy_player_array(PlayerArray *dst_player_array, const PlayerArray *src_player_array);
void copy_player_array_element(PlayerArray *dst_player_array, const PlayerArray *src_player_array);
void print_player_array(const PlayerArray *player_array);
void destroy_local_player_array(PlayerArray *player_array);

#endif
