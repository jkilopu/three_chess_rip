#include "map.h"
#include "road.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_empty_local_map(Map *map, unsigned int h, unsigned int w)
{
    to_map_pos(h, w);

    map->m = malloc(sizeof(MapElem *) * h);
    for (unsigned int i = 0; i < h; i++)
    {
        map->m[i] = malloc(sizeof(MapElem) * w);
        for (unsigned int j = 0; j < w; j++)
        {
            if (i % 2 == 1 && j % 2 == 1)
                map->m[i][j].chess_record.p_idx = NULL_PLAYER_IDX;
            else
                map->m[i][j].road = NULL_ROAD;
        }
    }
}

void clear_map(Map *map)
{
    for (unsigned int i = 0; i < map->h; i++)
    {
        for (unsigned int j = 0; j < map->w; j++)
        {
            if (i % 2 == 1 && j % 2 == 1)
                map->m[i][j].chess_record.p_idx = NULL_PLAYER_IDX;
            else
                map->m[i][j].road = NULL_ROAD;
        }
    }
}

Map *create_copy_of_map(Map *src_map)
{
    Map *new_map = malloc(sizeof(Map));

    new_map->w = src_map->w;
    new_map->h = src_map->h;

    new_map->m = malloc(sizeof(MapElem *) * src_map->h);
    for (unsigned int i = 0; i < src_map->h; i++)
    {
        new_map->m[i] = malloc(sizeof(MapElem) * src_map->w);
        memcpy(new_map->m[i], src_map->m[i], src_map->w * sizeof(MapElem));
    }

    return new_map;
}

static void init_players_in_map(Map *map, PlayerArray *player_array)
{
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        for (ChessIdx c_i = 0; c_i < player_array->players[p_i].chess_num; c_i++)
        {
            unsigned int y = map_val(player_array->players[p_i].chesses[c_i].pos.y);
            unsigned int x = map_val(player_array->players[p_i].chesses[c_i].pos.x);
            map->m[y][x].chess_record.p_idx = p_i;
            map->m[y][x].chess_record.c_idx = c_i;
        }
    }
}

static void init_roads_in_map(Map *map, const Path paths[], unsigned int path_num)
{
    for (unsigned int i = 0; i < path_num; i++)
    {
        unsigned int y = map_val(paths[i].first.y + paths[i].second.y);
        unsigned int x = map_val(paths[i].first.x + paths[i].second.x);
        set_path(map->m[y][x].road);

        if (paths[i].second.y != paths[i].first.y && paths[i].second.x != paths[i].first.x)
        {
            if (((int) paths[i].second.y - (int) paths[i].first.y) * ((int) paths[i].second.x * (int )paths[i].first.x) > 0) // 有溢出的可能
                set_go_up_left_or_down_right(map->m[y][x].road);
            else
                set_go_up_right_or_down_left(map->m[y][x].road);
        }
    }
}

void init_map(Map *map, PlayerArray *player_array, const Path paths[], unsigned int path_num)
{
    init_players_in_map(map, player_array);
    init_roads_in_map(map, paths, path_num);
}

void print_map(const Map *map)
{
    for (unsigned int i = 0; i < map->h; i++)
    {
        for (unsigned int j = 0; j < map->w; j++)
        {
            if (i % 2 == 0 || j % 2 == 0)
                printf("%hhu", map->m[i][j].road);
            else if(map->m[i][j].chess_record.p_idx == NULL_PLAYER_IDX)
                putchar(player_idx_to_char(map->m[i][j].chess_record.p_idx));
            else
                putchar('x');
            putchar(' ');
        }
        putchar('\n');
    }
}

void destroy_local_map(Map *map)
{
    for (unsigned int i = 0; i < map->h; i++)
    {
        free(map->m[i]);
        map->m[i] = NULL;
    }
    free(map->m);
    map->m = NULL;
}
