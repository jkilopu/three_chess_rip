#include "player_array.h"
#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void create_empty_local_player_array(PlayerArray *player_array, const ChessIdx chess_nums[], PlayerIdx player_num)
{
    player_array->players = malloc(sizeof(Player) * player_num);

    for (PlayerIdx p_i = 0; p_i < player_num; p_i++)
    {
        player_array->players[p_i].chesses = malloc(sizeof(Chess) * chess_nums[p_i]);
        player_array->players[p_i].out = false;
        player_array->players[p_i].chess_num = chess_nums[p_i];
        for (ChessIdx c_i = 0; c_i < chess_nums[p_i]; c_i++)
            player_array->players[p_i].chesses[c_i].pos = (Point2D) {0, 0};
    }

    player_array->player_num = player_num;
}

void clear_player_array(PlayerArray *player_array)
{
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        player_array->players[p_i].out = false;
        for (ChessIdx c_i = 0; c_i < player_array->players[p_i].chess_num; c_i++)
        {
            player_array->players[p_i].chesses[c_i].pos = (Point2D) {0, 0}; // 这也许不算清空？
        }
    }
}

void init_player_array(PlayerArray *player_array, const Point2D poses[])
{
    unsigned int pos_idx = 0;
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        player_array->players[p_i].out = false;
        for (ChessIdx c_i = 0; c_i < player_array->players[p_i].chess_num; c_i++)
        {
            player_array->players[p_i].chesses[c_i].pos = poses[pos_idx];
            pos_idx++;
        }
    }
}

void print_player_array(const PlayerArray *player_array)
{
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        printf("Player %c: ", player_idx_to_char(p_i));
        for (ChessIdx c_i = 0; c_i < player_array->players[p_i].chess_num; c_i++)
            printf("(%u, %u)", player_array->players[p_i].chesses[c_i].pos.y, player_array->players[p_i].chesses[c_i].pos.x);
        putchar('\n');
    }
    printf("Player alive:");
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
        if (!player_array->players[p_i].out)
            printf(" %c", player_idx_to_char(p_i));
    putchar('\n');
}

void destroy_local_player_array(PlayerArray *player_array)
{
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        free(player_array->players[p_i].chesses);
        player_array->players[p_i].chesses = NULL;
        player_array->players[p_i].chess_num = 0;
    }

    free(player_array->players);
    player_array->players = NULL;
}
