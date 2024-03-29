#include "robot.h"
#include "move.h"
#include "map.h"
#include "movement_idx.h"
#include <stdio.h>
#include <stdlib.h>

static void choose_pos_dir(DirectionIdx dir_idx, Point2D *p_pos, DirectionIdx *p_dir, Point2D movable_poses[], DirectionIdx movable_dirs[][DIRECTION_NUM], DirectionIdx dir_cnts[], ChessIdx movable_chess_num)
{
    ChessIdx chess_choice = 0;
    DirectionIdx dir_choice = dir_idx;
    for (ChessIdx c_i = 0; chess_choice < movable_chess_num; chess_choice++)
    {
        if (c_i + dir_cnts[chess_choice] > dir_choice)
        {
            dir_choice -= c_i;
            break;
        }
        c_i += dir_cnts[chess_choice];
    }

    *p_pos = movable_poses[chess_choice];
    *p_dir = movable_dirs[chess_choice][dir_choice];
}

RobotInfo robot_get_move_choices(Round *round, Point2D movable_poses[], DirectionIdx movable_dirs[][DIRECTION_NUM], DirectionIdx dir_cnts[], ChessIdx *p_movable_chess_num)
{
    *p_movable_chess_num = get_available_move_choice(round, movable_poses, movable_dirs, dir_cnts);

    if (*p_movable_chess_num == 0)
        return NO_AVAILABLE_CHOICE;
    if (*p_movable_chess_num > 6)
        return CHOICE_TOO_MUCH;

    return ROBOT_SUCCESS;
}

#include <stdlib.h>
#include <time.h>
RobotInfo robot_make_random_choice(Game *game, Point2D *p_pos, DirectionIdx *p_dir)
{
    Point2D movable_poses[3 * 2];
    DirectionIdx movable_dirs[3 * 2][DIRECTION_NUM];
    DirectionIdx dir_cnts[3 * 2];
    ChessIdx movable_chess_num;
    RobotInfo robot_info = robot_get_move_choices(&game->round, movable_poses, movable_dirs, dir_cnts, &movable_chess_num);

    if (robot_info == ROBOT_SUCCESS)
    {
        srand(time(NULL));

        DirectionIdx movable_dir_cnt = 0;
        for (ChessIdx c_i = 0; c_i < movable_chess_num; c_i++)
            movable_dir_cnt += dir_cnts[c_i];

        DirectionIdx dir_idx = rand() % movable_dir_cnt;
        choose_pos_dir(dir_idx, p_pos, p_dir, movable_poses, movable_dirs, dir_cnts, movable_chess_num);
    }

    return robot_info;
}

RobotInfo robot_make_best_choice(Game *game, Point2D *p_pos, DirectionIdx *p_dir)
{
    RobotInfo robot_info = ROBOT_SUCCESS;
    Node *cur_node = game->game_tree->cur_node;
    
    if (cur_node->node_status == COMPLETE_WIN)
    {
        KidIdx k_i = 0;
        while (k_i < cur_node->kid_num)
        {
            if (cur_node->kids[k_i]->node_status == COMPLETE_WIN)
            {
                *p_dir = idx_to_movement(cur_node->mi[k_i], p_pos, game->round.map.visable_h, game->round.map.visable_w);
                break;
            }
            k_i++;
        }
        if (k_i == cur_node->kid_num)
            robot_info = NO_COMPLETE_WIN_NODE;
    }
    else
    {
        KidIdx max_win_rate_kid_idx = 0;
        for (KidIdx k_i = 1; k_i < cur_node->kid_num; k_i++)
        {
            if ((double)cur_node->kids[k_i]->win_num / (double) cur_node->kids[k_i]->total_num
                    > (double) cur_node->kids[max_win_rate_kid_idx]->win_num / (double) cur_node->kids[max_win_rate_kid_idx]->total_num)
                max_win_rate_kid_idx = k_i;
        }
        *p_dir = idx_to_movement(cur_node->mi[max_win_rate_kid_idx], p_pos, game->round.map.visable_h, game->round.map.visable_w);
    }

    return robot_info;
}

void handle_robot_info(RobotInfo robot_info)
{
    switch (robot_info)
    {
        case NO_AVAILABLE_CHOICE:
            puts("RobotInfo: No available choice for robot.");
            exit(1);
        case ROBOT_SUCCESS:
            break;
        case CHOICE_TOO_MUCH:
            puts("RobotInfo: Having so much choice is impossible.");
            exit(1);
        case NO_COMPLETE_WIN_NODE:
            puts("RobotInfo: Can't find complete win kid.");
            exit(1);
        default:
            puts("RobotInfo: Unknown robot error. Exit.");
            exit(1);
    }
}
