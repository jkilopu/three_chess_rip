#include "chess_idx.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    PlayerIdx parr[] = {0xff, 0xff, 0xff, 2, 2, 2, 1, 1, 1, 0, 0, 0};
    Player players[] = {
        {NULL, 3, false},
        {NULL, 3, false},
        {NULL, 3, false},
    };
    PlayerArray player_array = {players, 3};

    create_and_init_combinations_state(&player_array, 12);
    printf("%u\n", idx_of_players_combinations(parr, &player_array));
    destroy_combinations_state();
}
