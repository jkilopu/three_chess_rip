#include "round.h"
#include "map.h"
#include "player.h"
#include "player_array.h"
#include "move.h"
#include <stdio.h>
#include <stdbool.h>

void create_empty_local_round(Round *round, unsigned int map_h, unsigned int map_w, const ChessIdx chess_nums[], PlayerIdx player_num)
{
    create_empty_local_map(&round->map, map_h, map_w);
    create_empty_local_player_array(&round->player_array, chess_nums, player_num);
    round->round_player_idx = NULL_PLAYER_IDX;
    round->out_player_idx = NULL_PLAYER_IDX;
    round->out_chess_moved = false;
    round->round_num = 0;
}

void init_round(Round *round, const Point2D poses[], const Path paths[], unsigned int path_num, PlayerIdx start_player_idx)
{
    init_player_array(&round->player_array, poses);
    init_map(&round->map, &round->player_array, paths, path_num);
    round->round_player_idx = start_player_idx;
    round->out_player_idx = NULL_PLAYER_IDX;
    round->out_chess_moved = false;
    round->round_num = 0;
}

void clear_round(Round *round)
{
    clear_map(&round->map);
    clear_player_array(&round->player_array);
    round->round_player_idx = NULL_PLAYER_IDX;
    round->out_player_idx = NULL_PLAYER_IDX;
    round->out_chess_moved = false;
    round->round_num = 0;
}

RoundInfo start_round(Round *round, Point2D pos, Direction dir)
{
    RoundInfo round_info = { .status=GAME_RUN };

    if (!move(round, pos, dir))
    {
        round_info.status = MOVE_FAIL;
        return round_info;
    }

    PlayerIdx last_player_idx = player_out(round);
    if (last_player_idx != NULL_PLAYER_IDX)
    {
        round_info.status = GAME_END;
        round_info.game_end.last_player_idx = last_player_idx;
    }

    return round_info;
}

void print_round(const Round *round)
{
    printf("Round %u\n", round->round_num);
    print_map(&round->map);
    print_player_array(&round->player_array);
    if (round->out_player_idx != NULL_PLAYER_IDX)
        printf("Double chess belongs to %c, moved: %hhu", player_idx_to_char(round->out_player_idx), round->out_chess_moved);
    printf("Player %c's turn: ", player_idx_to_char(round->round_player_idx));
}

void destroy_local_round(Round *round)
{
    destroy_local_map(&round->map);
    destroy_local_player_array(&round->player_array);
    round->round_player_idx = NULL_PLAYER_IDX;
    round->out_player_idx = NULL_PLAYER_IDX;
    round->out_chess_moved = false;
    round->round_num = 0;
}
