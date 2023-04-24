#include "round.h"
#include "map.h"
#include "player.h"
#include "player_array.h"
#include "move.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void create_empty_local_round(Round *round, unsigned int map_visable_h, unsigned int map_visable_w, const ChessIdx chess_nums[], PlayerIdx player_num)
{
    create_empty_local_map(&round->map, map_visable_h, map_visable_w);
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

Round *create_copy_of_round(Round *src_round)
{
    Round *new_round = malloc(sizeof(Round));
    create_copy_of_local_map(&new_round->map, &src_round->map);
    create_copy_of_local_player_array(&new_round->player_array, &src_round->player_array);
    new_round->round_player_idx = src_round->round_player_idx;
    new_round->out_player_idx = src_round->out_player_idx;
    new_round->out_chess_moved = src_round->out_chess_moved;
    new_round->round_num = src_round->round_num;
    return new_round;
}

void copy_round_dynamic_part(Round *dst_round, Round *src_round)
{
    copy_map_element(&dst_round->map, &src_round->map);
    copy_player_array_element(&dst_round->player_array, &src_round->player_array);
    dst_round->round_player_idx = src_round->round_player_idx;
    dst_round->out_player_idx = src_round->out_player_idx;
    dst_round->out_chess_moved = src_round->out_chess_moved;
    dst_round->round_num = src_round->round_num;
}

RoundInfo start_round(Round *round, Point2D pos, DirectionIdx dir)
{
    RoundInfo round_info = { .status=GAME_RUN };

    if (!move(round, pos, dir))
    {
        round_info.status = MOVE_FAIL;
        return round_info;
    }

    PlayerIdx out_cnt = 0;
    PlayerIdx last_move_player_idx = round->round_player_idx;
    do
    {
        round->round_player_idx = (round->round_player_idx + 1) % round->player_array.player_num;
        if (round->player_array.players[round->round_player_idx].out || player_out(round))
            out_cnt++;
        else
            break;
    }
    while (out_cnt < round->player_array.player_num - 1);

    if (out_cnt >= round->player_array.player_num - 1)
    {
        round_info.status = GAME_END;
        round_info.game_end.last_player_idx = last_move_player_idx;
    }

    round->round_num++;

    return round_info;
}

void print_round(const Round *round)
{
    printf("\nRound %u\n", round->round_num);
    print_map(&round->map);
    print_player_array(&round->player_array);
    if (round->out_player_idx != NULL_PLAYER_IDX)
        printf("Double chess belongs to %c, moved: %hhu\n", player_idx_to_char(round->out_player_idx), round->out_chess_moved);
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

void destroy_round(Round *round)
{
    destroy_local_round(round);
    free(round);
}
