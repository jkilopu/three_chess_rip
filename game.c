#include "game.h"
#include "robot.h"
#include "error.h"
#include "map.h"
#include "map_def.h"
#include "input.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

Game g_game;

static Game saved_game;

void setup(void)
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);
}

static void init_chess_records(void)
{
    unsigned int cnts[CHESS_PER_PLAYER] = {};
    for (unsigned int i = 0; i < g_game.map->h; i += 2)
        for (unsigned int j = 0; j < g_game.map->w; j += 2)
            if (g_game.map->m[i][j].chess.player != NULL_PLAYER)
            {
                Player player_idx = PLAYER_IDX(g_game.map->m[i][j].chess.player);
                g_game.chess_records[player_idx][cnts[player_idx]++] = (Point2D) {i / 2, j / 2}; // 这里必须要强制转换，否则编译器会认为是数组的初始化
            }
}

static void print_chess_records(void)
{
    for (Player i = FIRST_PLAYER; i <= LAST_PLAYER; i++)
    {
        printf("Player %c:", PLAYER_TO_CHAR(i));
        for (ChessIdx j = 0; j < CHESS_PER_PLAYER; j++)
            printf(" (%u, %u)", g_game.chess_records[PLAYER_IDX(i)][j].y, g_game.chess_records[PLAYER_IDX(i)][j].x);
        putchar('\n');
    }
}

void create_empty_game(void)
{
    g_game.map = create_empty_map(3, 4);
}

void init_game(void)
{
    static const Point2D initial_chess_records[PLAYER_NUM][CHESS_PER_PLAYER] = {
        { {0, 0}, {0, 1}, {0, 2}, },
        { {1, 0}, {1, 1}, {1, 2}, },
        { {2, 0}, {2, 1}, {2, 2}, },
    };    

    static Path paths[] = {
        { {0, 0}, {0, 1} },
        { {1, 0}, {1, 1} },
        { {2, 0}, {2, 1} },
        { {0, 1}, {0, 2} },
        { {1, 1}, {1, 2} },
        { {2, 1}, {2, 2} },
        { {0, 2}, {0, 3} },
        { {1, 2}, {1, 3} },
        { {2, 2}, {2, 3} },
        { {0, 1}, {1, 1} },
        { {0, 2}, {1, 2} },
        { {0, 3}, {1, 3} },
        { {1, 1}, {2, 1} },
        { {1, 2}, {2, 2} },
        { {1, 3}, {2, 3} },
        { {1, 2}, {0, 3} },
        { {1, 2}, {2, 3} },
    }; 

    memcpy(g_game.chess_records, initial_chess_records, sizeof(g_game.chess_records));
    init_map(g_game.map, g_game.chess_records, paths, sizeof(paths) / sizeof(Path));

    init_chess_records();

    g_game.round_player = NULL_PLAYER;
    g_game.user_player = NULL_PLAYER;
    g_game.out_player = NULL_PLAYER;
    g_game.out_chess_moved = false;
    memset(g_game.status, 0, sizeof(g_game.status));
    g_game.round_num = 0;
}

void save_game(void)
{
    memcpy(&saved_game, &g_game, sizeof(Game));
    saved_game.map = create_copy_of_map(g_game.map);
}

void restore_game(void)
{
    destroy_map(g_game.map); // 可以出UAF?
    memcpy(&g_game, &saved_game, sizeof(Game));
}

static void print_move_choice(Point2D pos, Direction dir)
{
    printf("(%u, %u) ", pos.y, pos.x);
    switch (dir)
    {
        case UP:
            puts("up");
            break;
        case DOWN:
            puts("down");
            break;
        case LEFT:
            puts("left");
            break;
        case RIGHT:
            puts("right");
            break;
        case UP_LEFT:
            puts("up-left");
            break;
        case UP_RIGHT:
            puts("up-right");
            break;
        case DOWN_LEFT:
            puts("down-left");
            break;
        case DOWN_RIGHT:
            puts("down-right");
            break;
        default:
            puts("Unknown direction");
            break;
    }
}

static Direction ask_for_choice(Player player, Point2D *p_pos, bool robot_playing)
{
    Direction dir; // 变量未初始化导致robot做出错误的选择？

    if (robot_playing)
    {
            RobotError robot_error = robot_make_unique_choice(p_pos, &dir);
            print_robot_error(robot_error);
    }
    else
    {
        printf("Player %c's turn: ", PLAYER_TO_CHAR(player));
        if (player == g_game.user_player)
        {
            char input_buf[0x20];
            read_n(input_buf, 0x20);
            dir = parse_move_input(input_buf, p_pos);
        }
        else
        {
            RobotError robot_error = robot_make_random_choice(p_pos, &dir);
            print_robot_error(robot_error);
            print_move_choice(*p_pos, dir);
        }
    }
    return dir;
}

static void print_game_info(void)
{
    print_map(g_game.map);
    print_chess_records();
}

void setup_game(Player user_player, Player start_player)
{
    g_game.user_player = user_player;
    g_game.round_player = start_player;
}

void setup_game_from_user_input(void)
{
    print_game_info();

    Player user_player = get_player("Choose your player(a, b or c): ");
    Player start_player = get_player("Select player who moves his chess first(a, b or c): ");
    setup_game(user_player, start_player);
}

bool start_round(bool robot_playing)
{
    if (!is_out(g_game.status[PLAYER_IDX(g_game.round_player)]))
    {
        while (true)
        {
            Point2D pos = {0, 0};
            Direction dir = ask_for_choice(g_game.round_player, &pos, robot_playing);
            if (move(&g_game, pos, dir))
                break;
        }
        player_lose(&g_game);
        if (!robot_playing)
            print_game_info();
    }

    unsigned int out_cnt = 0;
    for (Player p = FIRST_PLAYER; p <= LAST_PLAYER; p++)
        out_cnt += is_out(g_game.status[PLAYER_IDX(p)]);

    return out_cnt == PLAYER_NUM - 1;
}

void game_loop(void)
{
    for (Player i = g_game.round_player; !start_round(false); g_game.round_num++)
    {
        i = (PLAYER_IDX(i) + 1) % 3 + FIRST_PLAYER;
        g_game.round_player = i;
    }
}

void finish_game(void)
{
    destroy_map(g_game.map);
    g_game.map = NULL;
}
