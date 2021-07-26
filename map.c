#include "map.h"
#include "road.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_empty_local_map(Map *map, unsigned int h, unsigned int w)
{
    map->visable_h = h;
    map->visable_w = w;
    map->inner_h = inner_val(h);
    map->inner_w = inner_val(w);

    map->m = malloc(sizeof(MapElem *) * map->inner_h);
    for (unsigned int i = 0; i < map->inner_h; i++)
    {
        map->m[i] = malloc(sizeof(MapElem) * map->inner_w);
        for (unsigned int j = 0; j < map->inner_w; j++)
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
    for (unsigned int i = 0; i < map->inner_h; i++)
    {
        for (unsigned int j = 0; j < map->inner_w; j++)
        {
            if (i % 2 == 1 && j % 2 == 1)
                map->m[i][j].chess_record.p_idx = NULL_PLAYER_IDX;
            else
                map->m[i][j].road = NULL_ROAD;
        }
    }
}

void create_copy_of_local_map(Map *new_map, const Map *src_map)
{
    new_map->m = malloc(sizeof(MapElem *) * src_map->inner_h);
    for (unsigned int i = 0; i < src_map->inner_h; i++)
    {
        new_map->m[i] = malloc(sizeof(MapElem) * src_map->inner_w);
        memcpy(new_map->m[i], src_map->m[i], src_map->inner_w * sizeof(MapElem));
    }
    new_map->inner_h = src_map->inner_h;
    new_map->inner_w = src_map->inner_w;
    new_map->visable_h = src_map->visable_h;
    new_map->visable_w = src_map->visable_w;
}

void copy_map(Map *dst_map, const Map *src_map)
{
    copy_map_element(dst_map, src_map);
    dst_map->inner_h = src_map->inner_h;
    dst_map->inner_w = src_map->inner_w;
    dst_map->visable_h = src_map->visable_h;
    dst_map->visable_w = src_map->visable_w;
}

void copy_map_element(Map *dst_map, const Map *src_map)
{
    for (unsigned int i = 0; i < src_map->inner_h; i++)
        memcpy(dst_map->m[i], src_map->m[i], src_map->inner_w * sizeof(MapElem));
}

static void init_players_in_map(Map *map, PlayerArray *player_array)
{
    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        for (ChessIdx c_i = 0; c_i < player_array->players[p_i].chess_num; c_i++)
        {
            unsigned int y = inner_val(player_array->players[p_i].chesses[c_i].pos.y);
            unsigned int x = inner_val(player_array->players[p_i].chesses[c_i].pos.x);
            map->m[y][x].chess_record.p_idx = p_i;
            map->m[y][x].chess_record.c_idx = c_i;
        }
    }
}

static void init_roads_in_map(Map *map, const Path paths[], unsigned int path_num)
{
    for (unsigned int i = 0; i < path_num; i++)
    {
        unsigned int y = paths[i].first.y + paths[i].second.y + 1;
        unsigned int x = paths[i].first.x + paths[i].second.x + 1;
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
    for (unsigned int i = 0; i < map->inner_h; i++)
    {
        for (unsigned int j = 0; j < map->inner_w; j++)
        {
            if (i % 2 == 0 || j % 2 == 0)
                printf("%hhu", map->m[i][j].road);
            else if(map->m[i][j].chess_record.p_idx != NULL_PLAYER_IDX)
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
    for (unsigned int i = 0; i < map->inner_h; i++)
    {
        free(map->m[i]);
        map->m[i] = NULL;
    }
    free(map->m);
    map->m = NULL;
}

void destroy_map(Map *map)
{
    destroy_local_map(map);
    free(map);
    map->visable_h = 0;
    map->visable_w = 0;
    map->inner_h = 0;
    map->inner_w = 0;
}
