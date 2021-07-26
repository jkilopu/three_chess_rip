#ifndef _CHESS_BIJECTION_H
#define _CHESS_BIJECTION_H

#include "round_def.h"
#include "player_def.h"
#include "chess_def.h"

void create_and_init_combinations_state(PlayerArray *player_array, unsigned int map_real_size);
unsigned int idx_of_players_combinations(const PlayerIdx parr[], const PlayerArray *player_array);
unsigned int idx_of_round(const Round *round);
unsigned int get_round_possible_states_num(void);
void destroy_combinations_state(void);

#endif
