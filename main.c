// Compile: gcc map.c logic.c error.c input.c robot.c game.c main.c -o main -g

#include "main.h"
#include "game.h"
#include <stdio.h>

int main(void)
{
    setup();

    const ChessIdx chess_num[] = {3, 3, 3};
    Game *game = create_empty_game(3, 4, chess_num, 3);

    setup_game_from_user_input(game);

    game_loop(game);

    destroy_game(game);

    return 0;
}

void setup(void)
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}
