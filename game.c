#include "game.h"
#include "round.h"
#include "settings.h"
#include "move.h"
#include "robot.h"
#include "game_tree.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>

Game *create_empty_game(unsigned int map_visable_h, unsigned int map_visable_w, const ChessIdx chess_nums[], PlayerIdx player_num)
{
    Game *new_game = malloc(sizeof(Game));
    create_empty_local_round(&new_game->round, map_visable_h, map_visable_w, chess_nums, player_num);
    clear_settings(&new_game->settings);
    new_game->game_tree = NULL;
    return new_game;
}

void init_game(Game *game, const Point2D poses[], const Path paths[], unsigned int path_num, PlayerIdx start_player_idx, PlayerIdx user_player_idx)
{
    init_round(&game->round, poses, paths, path_num, start_player_idx);
    init_settings(&game->settings, user_player_idx);
    game->round.out_player_idx = 2;
    game->round.player_array.players[2].out = true;
    init_game_tree_in_game(game);
}

void setup_game_from_user_input(Game *game)
{
    //static const Path paths[] = {
    //    { {0, 0}, {0, 1} },
    //    { {1, 0}, {1, 1} },
    //    { {2, 0}, {2, 1} },
    //    { {0, 1}, {0, 2} },
    //    { {1, 1}, {1, 2} },
    //    { {2, 1}, {2, 2} },
    //    { {0, 2}, {0, 3} },
    //    { {1, 2}, {1, 3} },
    //    { {2, 2}, {2, 3} },
    //    { {0, 1}, {1, 1} },
    //    { {0, 2}, {1, 2} },
    //    { {0, 3}, {1, 3} },
    //    { {1, 1}, {2, 1} },
    //    { {1, 2}, {2, 2} },
    //    { {1, 3}, {2, 3} },
    //    { {1, 2}, {0, 3} },
    //    { {1, 2}, {2, 3} },
    //}; 

    static const Path paths[] = {
        { {0, 0}, {0, 1} },
        { {1, 0}, {1, 1} },
        { {2, 0}, {2, 1} },
        { {0, 1}, {1, 2} },
        { {1, 1}, {1, 2} },
        { {2, 1}, {1, 2} },
        { {0, 1}, {1, 1} },
        { {2, 1}, {1, 1} },
    }; 

    //static const Point2D poses[] = {
    //    {0, 0}, {0, 1}, {0, 2},
    //    {1, 0}, {1, 1}, {1, 2},
    //    {2, 0}, {2, 1}, {2, 2},
    //};
    static const Point2D poses[] = {
        {0, 0}, {0, 1},
        {2, 0}, {2, 1},
        {1, 1},
    };


    PlayerIdx user_player_idx = get_player_idx("Choose your player: ", game->round.player_array.player_num);
    PlayerIdx start_player_idx = get_player_idx("Select player who moves his chess first(a, b or c): ", game->round.player_array.player_num);
    init_game(game, poses, paths, sizeof(paths) / sizeof(Path), start_player_idx, user_player_idx);
}

static DirectionIdx ask_for_choice(Game *game, Point2D *p_pos)
{
    DirectionIdx dir; // 变量未初始化导致robot做出错误的选择？

    if (game->round.round_player_idx == game->settings.user_player_idx)
    {
        MoveInfo move_info;
        do
        {
            char input_buf[0x20];
            read_n(input_buf, 0x20);
            dir = parse_move_input(input_buf, p_pos);
            move_info = move_try(&game->round, *p_pos, dir, false);
            handle_move_info(move_info);
        }
        while (move_info != MOVE_SUCCESS);
    }
    else
    {
        RobotInfo robot_info = robot_make_random_choice(game, p_pos, &dir);
        handle_robot_info(robot_info);
        print_move_choice(*p_pos, dir);
    }

    return dir;
}

void game_loop(Game *game)
{
    RoundInfo round_info;
    do
    {
        print_round(&game->round);

        Point2D pos;
        DirectionIdx dir = ask_for_choice(game, &pos);

        step_on_game_tree(game->game_tree, pos, dir, game->round.map.visable_h, game->round.map.visable_w);

        round_info = start_round(&game->round, pos, dir);
    }
    while (round_info.status != GAME_END);
}

void destroy_game(Game *game)
{
    destroy_local_round(&game->round);
    free(game);
}
