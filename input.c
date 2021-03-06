#include "input.h"
#include "player.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

PlayerIdx get_player_idx(const char *msg, PlayerIdx player_num)
{
    char buf[2];

    do
    {
        printf(msg); // 潜在的格式化字符串漏洞？
        read_n(buf, 2);
    }
    while (buf[0] < player_idx_to_char(0) || buf[0] > player_idx_to_char(player_num - 1) );

    return buf[0] - FIRST_PLAYER_IDX_CHAR;
}

void read_n(char *buf, unsigned int len)
{
    char *p = buf;
    int ret;

    for (unsigned int i = 0; i < len; i++)
    {
        if ((ret = read(0, p, 1)) == -1)
        {
            printf("Read error. Exit.");
            exit(1);
        }
        else if (ret == 0)
            break;
        else
        {
            if (*p == '\n')
            {
                *p = '\x00';
                break;
            }
            p++;
        }
    }
}

DirectionIdx parse_move_input(const char buf[], Point2D *p_pos)
{
    char *dir_p = strstr(buf, ") ");
    if (dir_p == NULL)
        return INVALID_DIRECTION;
    *(dir_p + 1) = '\0';
    dir_p += 2;

    sscanf(buf, "(%u, %u)", &p_pos->y, &p_pos->x);

    if (strcmp(dir_p, "up") == 0)
        return UP;
    if (strcmp(dir_p, "down") == 0)
        return DOWN;
    if (strcmp(dir_p, "left") == 0)
        return LEFT;
    if (strcmp(dir_p, "right") == 0)
        return RIGHT;
    if (strcmp(dir_p, "up-left") == 0)
        return UP_LEFT;
    if (strcmp(dir_p, "up-right") == 0)
        return UP_RIGHT;
    if (strcmp(dir_p, "down-left") == 0)
        return DOWN_LEFT;
    if (strcmp(dir_p, "down-right") == 0)
        return DOWN_RIGHT;
    return INVALID_DIRECTION;
}
