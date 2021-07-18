#ifndef _GAME_H
#define _GAME_H

#include "map_def.h"
#include <stdbool.h>

void setup(void);
void create_empty_game(void);
void init_game(void);
void setup_game(Player user_player, Player start_player);
void setup_game_from_user_input(void);
void save_game(void);
void restore_game(void);
bool start_round(bool robot_playing);
void game_loop(void);
void finish_game(void);

#endif
