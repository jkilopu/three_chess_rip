#ifndef _GAME_DEF_H
#define _GAME_DEF_H

#include "round_def.h"
#include "settings_def.h"

typedef struct {
    Round round;
    Settings settings;
} Game;

#endif
