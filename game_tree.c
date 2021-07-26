/**
 * 现在的问题：
 * 1. game state太多了，user_player和out_player也要算在内吗
 * 2. 如何创建博弈树
 * 3. Game中的state似乎可以删去。
 *
 * */
#include "game_tree.h"
#include "round.h"
#include "move.h"
#include "chess_idx.h"
#include "movement_idx.h"
#include "robot.h"
#include "map.h"
#include <stdlib.h>

static Node **node_records;

static void create_node_records(Round *round)
{
    create_and_init_combinations_state(&round->player_array, round->map.visable_h * round->map.visable_w);
    node_records = calloc(get_round_possible_states_num(), sizeof(Node *));
}

static void destroy_node_records(void)
{
    destroy_combinations_state();
    free(node_records);
    node_records = NULL;
}

static void add_node(Game *game, Node *parent)
{
    Point2D movable_poses[3 * 2];
    DirectionIdx movable_dirs[3 * 2][DIRECTION_NUM];
    DirectionIdx dir_cnts[3 * 2];
    ChessIdx movable_chess_num;

    RobotInfo robot_info = robot_get_move_choices(&game->round, movable_poses, movable_dirs, dir_cnts, &movable_chess_num);
    handle_robot_info(robot_info);

    unsigned int total_choice_num = 0;
    for (ChessIdx c_i = 0; c_i < movable_chess_num; c_i++)
        total_choice_num += dir_cnts[c_i];
    parent->kid_num = total_choice_num;
    parent->mi = malloc(sizeof(MovementIdx) * total_choice_num);
    parent->kids = malloc(sizeof(Node *) * total_choice_num);
    parent->win_num = 0;
    parent->total_num = 0;
    parent->node_status = UNDETERMINED;

    Round *saved_round = create_copy_of_round(&game->round);
    for (ChessIdx c_i = 0; c_i < movable_chess_num; c_i++)
    {
        Point2D pos = movable_poses[c_i];
        for (DirectionIdx d_i = 0; d_i < dir_cnts[c_i]; d_i++) // 这里dir_cnts[c_i]会被单独优化为一个变量么
        {
            DirectionIdx dir = movable_dirs[c_i][d_i];
            MovementIdx movement_idx = idx_of_movement(pos, dir, game->round.map.visable_h, game->round.map.visable_w);

            total_choice_num--;
            parent->mi[total_choice_num] = movement_idx;

            RoundInfo round_info = start_round(&game->round, pos, dir);
            unsigned int round_idx = idx_of_round(&game->round);

            if (node_records[round_idx] != NULL)
                parent->kids[total_choice_num] = node_records[round_idx];
            else
            {
                Node *kid = malloc(sizeof(Node));
                node_records[round_idx] = kid;
                parent->kids[total_choice_num] = kid;
                if (round_info.status == GAME_END)
                {
                    kid->mi = NULL;
                    kid->kids = NULL;
                    kid->kid_num = 0;
                    kid->node_status = (round_info.game_end.last_player_idx == game->settings.user_player_idx ? NOT_COMPLETE_WIN : COMPLETE_WIN);
                    kid->win_num = (round_info.game_end.last_player_idx == game->settings.user_player_idx ? 0 : 1);
                    kid->total_num = 1;
                }
                else
                    add_node(game, kid);
            }

            copy_round_dynamic_part(&game->round, saved_round);
        }
    }

    destroy_round(saved_round);

    if (game->round.round_player_idx == game->settings.user_player_idx)
    {
        bool complete_win = true;
        bool all_kids_undetermined = true;
        for (KidIdx k_i = 0; k_i < parent->kid_num; k_i++)
        {
            if (parent->kids[k_i]->node_status != UNDETERMINED)
            {
                all_kids_undetermined = false;
                if (parent->kids[k_i]->node_status == NOT_COMPLETE_WIN)
                    complete_win = false;
                parent->win_num += parent->kids[k_i]->win_num;
                parent->total_num += parent->kids[k_i]->total_num;
            }
        }
        parent->node_status = all_kids_undetermined ? UNDETERMINED : complete_win ? COMPLETE_WIN : NOT_COMPLETE_WIN;
    }
    else
    {
        bool complete_win = false;
        bool all_kids_undetermined = true;
        for (KidIdx k_i = 0; k_i < parent->kid_num; k_i++)
        {
            if (parent->kids[k_i]->node_status != UNDETERMINED)
            {
                all_kids_undetermined = false;
                if (parent->kids[k_i]->node_status == COMPLETE_WIN)
                    complete_win = true;
                parent->win_num += parent->kids[k_i]->win_num;
                parent->total_num += parent->kids[k_i]->total_num;
            }
        }
        parent->node_status = all_kids_undetermined ? UNDETERMINED : complete_win ? COMPLETE_WIN : NOT_COMPLETE_WIN;
    }
}

void *init_game_tree_in_game(Game *game)
{
    create_node_records(&game->round);

    game->game_tree = malloc(sizeof(GameTree));
    game->game_tree->start_node = malloc(sizeof(Node));
    game->game_tree->cur_node = game->game_tree->start_node;
    node_records[idx_of_round(&game->round)] = game->game_tree->start_node;

    add_node(game, game->game_tree->start_node);

    destroy_node_records();
}

void step_on_game_tree(GameTree *game_tree, Point2D pos, DirectionIdx dir, unsigned int map_visable_h, unsigned int map_visable_w)
{
    Node *cur_node = game_tree->cur_node;
    MovementIdx mv_idx = idx_of_movement(pos, dir, map_visable_h, map_visable_w);

    for (KidIdx k_i = 0; k_i < cur_node->kid_num; k_i++)
        if (mv_idx == cur_node->mi[k_i])
        {
            game_tree->cur_node = cur_node->kids[k_i];
            break;
        }
}
