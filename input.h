#ifndef _INPUT_H
#define _INPUT_H

#include "logic.h"
#include "map_def.h"

Player get_player(const char *msg);
void read_n(char *buf, unsigned int len);
Direction parse_move_input(const char buf[], Point2D *p_pos);

#endif
