#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "settings_def.h"
#include "player_def.h"

void clear_settings(Settings *settings);
void init_settings(Settings *settings, PlayerIdx user_player_idx);

#endif
