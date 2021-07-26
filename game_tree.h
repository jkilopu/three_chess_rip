#ifndef _GAME_TREE_H
#define _GAME_TREE_H

#include "game_tree_def.h"
#include "game_def.h"
#include "point_def.h"
#include "direction_def.h"

void *init_game_tree_in_game(Game *game);
void step_on_game_tree(GameTree *game_tree, Point2D pos, DirectionIdx dir, unsigned int map_visable_h, unsigned int map_visable_w);

#endif
