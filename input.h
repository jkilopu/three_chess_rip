#ifndef _INPUT_H
#define _INPUT_H

#include "player_def.h"
#include "direction_def.h"
#include "point_def.h"

PlayerIdx get_player_idx(const char *msg, PlayerIdx player_num);
void read_n(char *buf, unsigned int len);
DirectionIdx parse_move_input(const char buf[], Point2D *p_pos);

#endif
