#ifndef _GAME_H
#define _GAME_H

#include "game_def.h"
#include "map_def.h" // For Path
#include "player_def.h"
#include "chess_def.h"
#include "point_def.h"
#include <stdbool.h>

Game *create_empty_game(unsigned int map_h, unsigned int map_w, const ChessIdx chess_nums[], PlayerIdx player_num);
void init_game(Game *game, const Point2D poses[], const Path paths[], unsigned int path_num, PlayerIdx start_player, PlayerIdx user_player);
void setup_game_from_user_input(Game *game);
void game_loop(Game *game);
void destroy_game(Game *game);

#endif
