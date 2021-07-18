#include "robot.h"
#include "game_tree.h"
#include "error.h"
#include "map.h"
#include "map_def.h"
#include "logic.h"

extern Game g_game;

static Direction movable_dirs[CHESS_PER_PLAYER * 2][DIRECTION_NUM];
static Point2D movable_poses[CHESS_PER_PLAYER * 2];
static unsigned int dir_cnts[CHESS_PER_PLAYER * 2];
static unsigned int movable_chess_num;
unsigned int movable_dir_cnt;
Direction current_dir;
Point2D current_pos;

static void choose_pos_dir(unsigned int idx, Point2D *p_pos, Direction *p_dir)
{
    unsigned int chess_choice = 0;
    unsigned int dir_choice = idx;
    for (unsigned int c = 0; chess_choice < movable_chess_num; chess_choice++)
    {
        if (c + dir_cnts[chess_choice] > dir_choice)
        {
            dir_choice -= c;
            break;
        }
        c += dir_cnts[chess_choice];
    }

    *p_pos = movable_poses[chess_choice];
    *p_dir = movable_dirs[chess_choice][dir_choice];
    current_dir = *p_dir;
    current_pos = *p_pos;
}

static RobotError get_choice_info(void)
{
    movable_chess_num = get_available_move_choice(&g_game, movable_poses, movable_dirs, dir_cnts);

    if (movable_chess_num == 0)
        return NO_AVAILABLE_CHOICE;

    movable_dir_cnt = 0;
    for (unsigned int i = 0; i < movable_chess_num; i++)
        movable_dir_cnt += dir_cnts[i];

    return ROBOT_SUCCESS;
}

#include <stdlib.h>
#include <time.h>
RobotError robot_make_random_choice(Point2D *p_pos, Direction *p_dir)
{
    RobotError robot_error = get_choice_info();

    if (robot_error == ROBOT_SUCCESS)
    {
        srand(time(NULL));

        unsigned int idx = rand() % movable_dir_cnt;
        choose_pos_dir(idx, p_pos, p_dir);
    }

    return robot_error;
}

RobotError robot_make_unique_choice(Point2D *p_pos, Direction *p_dir)
{
    RobotError robot_error = ROBOT_SUCCESS;

    if (movable_dir_cnt == 0)
        robot_error = get_choice_info();

    choose_pos_dir(--movable_dir_cnt, p_pos, p_dir);

    return robot_error;
}

RobotError robot_make_smart_choice(Point2D *p_pos, Direction *p_dir)
{
}
