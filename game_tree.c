
/**
 * 现在的问题：
 * 1. game state太多了，user_player和out_player也要算在内吗
 * 2. 如何创建博弈树
 * 3. Game中的state似乎可以删去。
 *
 * */
#include "game_tree.h"
#include "game.h"
#include "logic.h"
#include "map_def.h"

extern Game g_game;
extern unsigned int movable_dir_cnt; 
extern Point2D curruent_pos;
extern Direction current_dir;

static Node **states;

static MovementIdx idx_of_movement(Point2D pos, Direction dir, unsigned int h, unsigned int w)
{
    return dir * h * w + pos.y * w + pos.x;
}

static Direction idx_to_movement(MovementIdx movement_idx, qPoint2D *p_pos unsigned int h, unsigned int w)
{
    p_pos->x = movement_idx % (h * w);
    p_pos->y = (movement_idx / w) % h;
    return (Direction) movement_idx / (h * w)
}

static void create_robot_state(void)
{
    states = calloc(369600 * 3, sizeof(Node *));
}

static void destroy_robot_state(void)
{
    free(states);
    states = NULL;
}

static void add_node(Node *parent)
{
    unsigned int i = 0;
    do
    {
        bool game_end = start_round(true);
        Node *child = malloc(sizeof(Node));
        child->mi = idx_of_movement(current_pos, current_dir, g_game.h / 2, g_game.w / 2);

        if (!game_end)
        {
            add_node(child)
        }
        else
            child->next = NULL;
    } while(++i < movable_dir_cnt);
}

GameTree *create_game_tree(void)
{
    save_game();
    init_game();
    g_game.round_player = FIRST_PLAYER;
    g_game.user_player = FIRST_PLAYER;
    create_robot_state();

    GameTree *game_tree = malloc(sizeof(GameTree));
    game_tree->start_node = malloc(sizeof(Node));
    states[idx_of_map(g_game.map)] = game_tree_start_node;
    add_node(game_tree->start_node)

    destroy_robot_state();
    restore_game();
}
