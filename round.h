#ifndef _ROUND_H
#define _ROUND_H

#include "round_def.h"
#include "chess_def.h"
#include "player_def.h"
#include "direction_def.h"
#include "map_def.h"

void create_empty_local_round(Round *round, unsigned int map_visable_h, unsigned int map_visable_w, const ChessIdx chess_nums[], PlayerIdx player_num);
void init_round(Round *round, const Point2D poses[], const Path paths[], unsigned int path_num, PlayerIdx start_player_idx);
void clear_round(Round *round);
Round *create_copy_of_round(Round *src_round);
void copy_round_dynamic_part(Round *dst_round, Round *src_round);
RoundInfo start_round(Round *round, Point2D pos, DirectionIdx dir);
void print_round(const Round *round);
void destroy_local_round(Round *round);
void destroy_round(Round *round);

#endif
