#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

Map *create_empty_map(unsigned int h, unsigned int w)
{
    Map *new_map = malloc(sizeof(Map));

    h = h * 2 - 1;
    w = w * 2 - 1;
    new_map->h = h, new_map->w = w;

    new_map->m = malloc(sizeof(MapElem *) * h);
    for (unsigned int i = 0; i < h; i++)
        new_map->m[i] = calloc(w, sizeof(MapElem));

    return new_map;
}

Map *create_copy_of_map(Map *src_map)
{
    Map *new_map = malloc(sizeof(Map));

    new_map->w = src_map->w;
    new_map->h = src_map->h;

    memcpy(new_map->m, src_map->m, new_map->w * new_map->h * sizeof(MapElem));

    return new_map;
}

void init_map(Map *map, Point2D chess_records[][CHESS_PER_PLAYER], Path paths[], unsigned int paths_num)
{
    for (Player i = FIRST_PLAYER; i <= LAST_PLAYER; i++)
        for (ChessIdx j = 0; j < CHESS_PER_PLAYER; j++)
        {
            unsigned int y = chess_records[PLAYER_IDX(i)][j].y * 2;
            unsigned int x = chess_records[PLAYER_IDX(i)][j].x * 2;
            map->m[y][x].chess.player = i;
            map->m[y][x].chess.idx = j;
        }

    for (unsigned int i = 0; i < paths_num; i++)
    {
        unsigned int y = paths[i].first.y + paths[i].second.y;
        unsigned int x = paths[i].first.x + paths[i].second.x;
        SET_PATH(map->m[y][x].road);

        if (paths[i].second.y != paths[i].first.y && paths[i].second.x != paths[i].first.x)
        {
            if (((int) paths[i].second.y - (int) paths[i].first.y) * ((int) paths[i].second.x * (int )paths[i].first.x) > 0) // 有溢出的可能
                SET_GO_UP_LEFT_OR_DOWN_RIGHT(map->m[y][x].road);
            else
                SET_GO_UP_RIGHT_OR_DOWN_LEFT(map->m[y][x].road);
        }
    }
}

void print_map(const Map *map)
{
    for (unsigned int i = 0; i < map->h; i++)
    {
        for (unsigned int j = 0; j < map->w; j++)
        {
            if (i % 2 == 1 || j % 2 == 1)
                printf("%hhu", map->m[i][j].road);
            else if(map->m[i][j].chess.player == NULL_PLAYER)
                putchar('x');
            else
                putchar(PLAYER_TO_CHAR(map->m[i][j].chess.player));
            putchar(' ');
        }
        putchar('\n');
    }
}

void destroy_map(Map *map)
{
    for (unsigned int i = 0; i < map->h; i++)
    {
        free(map->m[i]);
        map->m[i] = NULL;
    }
    free(map->m);
    map->m = NULL;
    free(map);
}
