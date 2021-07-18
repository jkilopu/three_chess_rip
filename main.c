// Compile: gcc map.c logic.c error.c input.c robot.c game.c main.c -o main -g

#include "game.h"

int main(void)
{
    setup();

    create_empty_game();

    init_game();

    setup_game_from_user_input();

    game_loop();

    finish_game();

    return 0;
}
