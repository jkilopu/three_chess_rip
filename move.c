#include "move.h"
#include "map.h"
#include "road.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

static char *move_dir_str[] = {
    "up",
    "down",
    "left",
    "right",
    "up-left",
    "up-right",
    "down-left",
    "down-right",
};

MoveInfo move_try(Round *round, Point2D pos, DirectionIdx dir, bool move_on_success)
{
    MoveInfo move_info = MOVE_SUCCESS;
    unsigned int y = inner_val(pos.y);
    unsigned int x = inner_val(pos.x);

    if (y >= round->map.inner_h || x >= round->map.inner_w) // 可以设计溢出的漏洞
        return POS_OUT_OF_BOUND;

    // 这里可以出逻辑漏洞
    if (round->round_player_idx != round->map.m[y][x].chess_record.p_idx)
    {
        if (round->out_player_idx == NULL_PLAYER_IDX)
            return WRONG_PLAYER;
        else
        {
            if (round->out_player_idx != round->map.m[y][x].chess_record.p_idx)
                return NOT_DOUBLE_CHESS;
            else if (round->out_chess_moved)
                return DOUBLE_CHESS_MOVED_LAST_TIME;
        }
    }

    PlayerIdx prev_player_idx = round->map.m[y][x].chess_record.p_idx;
    ChessIdx prev_chess_idx = round->map.m[y][x].chess_record.c_idx;
    switch (dir)
    {
        case UP:
            if (!is_path(round->map.m[y - 1][x].road))
                move_info = UP_HAS_OBSTACLE;
            else if (round->map.m[y - 2][x].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y - 2][x].chess_record.p_idx = prev_player_idx;
                round->map.m[y - 2][x].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {(y - 2) / 2, x / 2};
            }
            break;
        case DOWN:
            if (!is_path(round->map.m[y + 1][x].road))
                move_info = DOWN_HAS_OBSTACLE;
            else if (round->map.m[y + 2][x].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y + 2][x].chess_record.p_idx = prev_player_idx;
                round->map.m[y + 2][x].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {(y + 2) / 2, x / 2};
            }
            break;
        case LEFT:
            if (!is_path(round->map.m[y][x - 1].road))
                move_info = LEFT_HAS_OBSTACLE;
            else if (round->map.m[y][x - 2].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y][x - 2].chess_record.p_idx = prev_player_idx;
                round->map.m[y][x - 2].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {y / 2, (x - 2) / 2};
            }
            break;
        case RIGHT:
            if (!is_path(round->map.m[y][x + 1].road))
                move_info = RIGHT_HAS_OBSTACLE;
            else if (round->map.m[y][x + 2].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y][x + 2].chess_record.p_idx = prev_player_idx;
                round->map.m[y][x + 2].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {y / 2, (x + 2) / 2};
            }
            break;
        case UP_LEFT:
            if (!is_path(round->map.m[y - 1][x - 1].road) || !can_go_up_left_or_down_right(round->map.m[y - 1][x - 1].road))
                move_info = UP_LEFT_HAS_OBSTACLE;
            else if (round->map.m[y - 2][x - 2].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y - 2][x - 2].chess_record.p_idx = prev_player_idx;
                round->map.m[y - 2][x - 2].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {(y - 2) / 2, (x - 2) / 2};
            }
            break;
        case UP_RIGHT:
            if (!is_path(round->map.m[y - 1][x + 1].road) || !can_go_up_right_or_down_left(round->map.m[y - 1][x + 1].road))
                move_info = UP_RIGHT_HAS_OBSTACLE;
            else if (round->map.m[y - 2][x + 2].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y - 2][x + 2].chess_record.p_idx = prev_player_idx;
                round->map.m[y - 2][x + 2].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {(y - 2) / 2, (x + 2) / 2};
            }
            break;
        case DOWN_LEFT:
            if (!is_path(round->map.m[y + 1][x - 1].road) || !can_go_up_right_or_down_left(round->map.m[y + 1][x - 1].road))
                move_info = DOWN_LEFT_HAS_OBSTACLE;
            else if (round->map.m[y + 2][x - 2].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y + 2][x - 2].chess_record.p_idx = prev_player_idx;
                round->map.m[y + 2][x - 2].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {(y + 2) / 2, (x - 2) / 2};
            }
            break;
        case DOWN_RIGHT:
            if (!is_path(round->map.m[y + 1][x + 1].road) || !can_go_up_left_or_down_right(round->map.m[y + 1][x + 1].road))
                move_info = DOWN_RIGHT_HAS_OBSTACLE;
            else if (round->map.m[y + 2][x + 2].chess_record.p_idx != NULL_PLAYER_IDX)
                move_info = HAS_CHESS;
            else if (move_on_success)
            {
                round->map.m[y + 2][x + 2].chess_record.p_idx = prev_player_idx;
                round->map.m[y + 2][x + 2].chess_record.c_idx = prev_chess_idx;
                round->player_array.players[prev_player_idx].chesses[prev_chess_idx].pos = (Point2D) {(y + 2) / 2, (x + 2) / 2};
            }
            break;
        default:
            move_info = UNKNOWN_OPERATION;
            break;
    }
    
    if (move_info == MOVE_SUCCESS && move_on_success) // 这里犯了把==写成=的错误
    {
        if (round->out_chess_moved)
            round->out_chess_moved = false;
        if (round->out_player_idx == round->map.m[y][x].chess_record.p_idx)
            round->out_chess_moved = true;
        round->map.m[y][x].chess_record.p_idx = NULL_PLAYER_IDX;
    }

    return move_info;
}

bool move(Round *round, Point2D pos, DirectionIdx dir)
{
    MoveInfo move_info = move_try(round, pos, dir, true);
    handle_move_info(move_info);
    return move_info == MOVE_SUCCESS;
}

static DirectionIdx get_movable_direction(Round *round, Point2D pos, DirectionIdx dir_arr[])
{
    DirectionIdx cnt = 0;

    for (DirectionIdx d = UP; d <= DOWN_RIGHT; d++)
        if (move_try(round, pos, d, false) == MOVE_SUCCESS)
            dir_arr[cnt++] = d;

    if (cnt != DIRECTION_NUM)
        dir_arr[cnt] = INVALID_DIRECTION;

    return cnt;
}

ChessIdx get_available_move_choice(Round *round, Point2D movable_poses[], DirectionIdx movable_dirs[][DIRECTION_NUM], DirectionIdx dir_cnts[])
{
    ChessIdx movable_chess_cnt = 0;

    PlayerIdx p_idx = round->round_player_idx;
    ChessIdx chess_num = round->player_array.players[p_idx].chess_num;
    for (ChessIdx c_i = 0; c_i < chess_num; c_i++)
    {
        Point2D pos = round->player_array.players[p_idx].chesses[c_i].pos;
        DirectionIdx movable_dir_cnt = get_movable_direction(round, pos, movable_dirs[movable_chess_cnt]);
        if (movable_dir_cnt)
        {
            movable_poses[movable_chess_cnt] = pos;
            dir_cnts[movable_chess_cnt] = movable_dir_cnt;
            movable_chess_cnt++;
        }
    }

    if (round->out_player_idx != NULL_PLAYER_IDX && !round->out_chess_moved)
    {
        p_idx = round->out_player_idx;
        ChessIdx chess_num = round->player_array.players[p_idx].chess_num;
        for (ChessIdx c_i = 0; c_i < chess_num; c_i++)
        {
            Point2D pos = round->player_array.players[p_idx].chesses[c_i].pos;
            DirectionIdx movable_dir_cnt = get_movable_direction(round, pos, movable_dirs[movable_chess_cnt]);
            if (movable_dir_cnt)
            {
                movable_poses[movable_chess_cnt] = pos;
                dir_cnts[movable_chess_cnt] = movable_dir_cnt;
                movable_chess_cnt++;
            }
        }
    }

    return movable_chess_cnt;
}

// 或许之后可以把它拆分为几个static函数
PlayerIdx player_out(Round *round)
{
    PlayerIdx player_num = round->player_array.player_num;
    bool lose[player_num];

    for (PlayerIdx p_i = 0; p_i < player_num; p_i++)
        lose[p_i] = true;

    PlayerIdx tmp_player_idx = round->round_player_idx;
    for (unsigned int y = 1; y < round->map.inner_h; y += 2)
        for (unsigned int x = 1; x < round->map.inner_w; x += 2)
            for (PlayerIdx p_i = 0; p_i < player_num; p_i++)
            {
                if (!round->player_array.players[p_i].out && round->map.m[y][x].chess_record.p_idx == p_i)
                {
                    Point2D pos = {visable_val(y), visable_val(x)};
                    round->round_player_idx = p_i;
                    for (DirectionIdx d = UP; d <= DOWN_RIGHT; d++)
                        if (move_try(round, pos, d, false) == MOVE_SUCCESS)
                        {
                            lose[p_i] = false;
                            break;
                        }
                    break;
                }
            }
    round->round_player_idx = tmp_player_idx;

    PlayerIdx out_cnt = 0;
    PlayerIdx last_player_idx = NULL_PLAYER_IDX;
    for (PlayerIdx p_i = 0; p_i < player_num; p_i++)
    {
        if (lose[p_i])
        {
            round->player_array.players[p_i].out = true;
            if (round->out_player_idx == NULL_PLAYER_IDX)
                round->out_player_idx = p_i;
        }

        if (!round->player_array.players[p_i].out)
            last_player_idx = p_i;
        else
            out_cnt++;
    }

    return out_cnt >= player_num - 1 ? last_player_idx : HAS_PLAYER_IDX;
}

void handle_move_info(MoveInfo move_info)
{
    switch (move_info)
    {
        case UNKNOWN_OPERATION:
            puts("MoveInfo: Unknown move operation.");
            break;
        case MOVE_SUCCESS:
            break;
        case POS_OUT_OF_BOUND:
            puts("MoveInfo: Pos out of bound.");
            break;
        case WRONG_PLAYER:
            puts("MoveInfo: Wrong player.");
            break;
        case DOUBLE_CHESS_MOVED_LAST_TIME:
            puts("MoveInfo: Double chess moved last time.");
            break;
        case NOT_DOUBLE_CHESS:
            puts("MoveInfo: Selected chess is not a double chess.");
            break;
        case HAS_CHESS:
            puts("MoveInfo: Destination has chess.");
            break;
        case UP_HAS_OBSTACLE:
        case DOWN_HAS_OBSTACLE:
        case LEFT_HAS_OBSTACLE:
        case RIGHT_HAS_OBSTACLE:
        case UP_LEFT_HAS_OBSTACLE:
        case UP_RIGHT_HAS_OBSTACLE:
        case DOWN_LEFT_HAS_OBSTACLE:
        case DOWN_RIGHT_HAS_OBSTACLE:
            printf("MoveInfo: Has obstacle at the %s direction.\n", move_dir_str[move_info - UP_HAS_OBSTACLE]);
            break;
        default:
            puts("MoveInfo: Unknown move error. Exit.");
            exit(1);
    }
}

void print_move_choice(Point2D pos, DirectionIdx dir)
{
    printf("(%u, %u) ", pos.y, pos.x);
    puts(move_dir_str[dir - UP]);
}
