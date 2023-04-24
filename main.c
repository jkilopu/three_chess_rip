/* Compile: gcc settings.c input.c map.c player_array.c round.c \
  robot.c game.c move.c movement_idx.c chess_idx.c game_tree.c main.c -o three_chess_rip -g */

#include "main.h"
#include "game.h"
#include <stdio.h>

int main(void)
{
    setup();

    const ChessIdx chess_nums[] = {2, 2, 1};
    Game *game = create_empty_game(3, 3, chess_nums, 3);

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
