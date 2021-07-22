#include "settings.h"

void clear_settings(Settings *settings)
{
    settings->user_player_idx = NULL_PLAYER_IDX;
}

void init_settings(Settings *settings, PlayerIdx user_player_idx)
{
    settings->user_player_idx = user_player_idx;
}
