#include "logic.h"
#include "game_def.h"
#include "error.h"
#include <stdbool.h>

static MoveError move_try(Game *game, Point2D pos, Direction dir, bool move_on_success)
{
    MoveError move_error = MOVE_SUCCESS;
    unsigned int y = pos.y * 2;
    unsigned int x = pos.x * 2;

    if (y >= game->map->h || x >= game->map->w) // 可以设计溢出的漏洞
        return POS_OUT_OF_BOUND;

    // 这里可以出逻辑漏洞
    if (game->round_player != game->map->m[y][x].chess.player)
    {
        if (game->out_player == NULL_PLAYER)
            return WRONG_PLAYER;
        else
        {
            if (game->out_player != game->map->m[y][x].chess.player)
                return NOT_DOUBLE_CHESS;
            else if (game->out_chess_moved)
                return DOUBLE_CHESS_MOVED_LAST_TIME;
        }
    }

    Player prev_player = game->map->m[y][x].chess.player;
    ChessIdx prev_chess_idx = game->map->m[y][x].chess.idx;
    switch (dir)
    {
        case UP:
            if (y == 0)
                move_error = OUT_OF_UP_BOUND;
            else if (!IS_PATH(game->map->m[y - 1][x].road))
                move_error = UP_HAS_OBSTACLE;
            else if (game->map->m[y - 2][x].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y - 2][x].chess.player = prev_player;
                game->map->m[y - 2][x].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {(y - 2) / 2, x / 2};
            }
            break;
        case DOWN:
            if (y == game->map->h - 1)
                move_error = OUT_OF_DOWN_BOUND;
            else if (!IS_PATH(game->map->m[y + 1][x].road))
                move_error = DOWN_HAS_OBSTACLE;
            else if (game->map->m[y + 2][x].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y + 2][x].chess.player = prev_player;
                game->map->m[y + 2][x].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {(y + 2) / 2, x / 2};
            }
            break;
        case LEFT:
            if (x == 0)
                move_error = OUT_OF_LEFT_BOUND;
            else if (!IS_PATH(game->map->m[y][x - 1].road))
                move_error = LEFT_HAS_OBSTACLE;
            else if (game->map->m[y][x - 2].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y][x - 2].chess.player = prev_player;
                game->map->m[y][x - 2].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {y / 2, (x - 2) / 2};
            }
            break;
        case RIGHT:
            if (x == game->map->w - 1)
                move_error = OUT_OF_RIGHT_BOUND;
            else if (!IS_PATH(game->map->m[y][x + 1].road))
                move_error = RIGHT_HAS_OBSTACLE;
            else if (game->map->m[y][x + 2].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y][x + 2].chess.player = prev_player;
                game->map->m[y][x + 2].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {y / 2, (x + 2) / 2};
            }
            break;
        case UP_LEFT:
            if (y == 0 || x == 0)
                move_error = OUT_OF_UP_LEFT_BOUND;
            else if (!IS_PATH(game->map->m[y - 1][x - 1].road) || !CAN_GO_UP_LEFT_OR_DOWN_RIGHT(game->map->m[y - 1][x - 1].road))
                move_error = UP_LEFT_HAS_OBSTACLE;
            else if (game->map->m[y - 2][x - 2].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y - 2][x - 2].chess.player = prev_player;
                game->map->m[y - 2][x - 2].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {(y - 2) / 2, (x - 2) / 2};
            }
            break;
        case UP_RIGHT:
            if (y == 0 || x == game->map->w - 1)
                move_error = OUT_OF_UP_RIGHT_BOUND;
            else if (!IS_PATH(game->map->m[y - 1][x + 1].road) || !CAN_GO_UP_RIGHT_OR_DOWN_LEFT(game->map->m[y - 1][x + 1].road))
                move_error = UP_RIGHT_HAS_OBSTACLE;
            else if (game->map->m[y - 2][x + 2].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y - 2][x + 2].chess.player = prev_player;
                game->map->m[y - 2][x + 2].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {(y - 2) / 2, (x + 2) / 2};
            }
            break;
        case DOWN_LEFT:
            if (y == game->map->h - 1 || x == 0)
                move_error = OUT_OF_DOWN_LEFT_BOUND;
            else if (!IS_PATH(game->map->m[y + 1][x - 1].road) || !CAN_GO_UP_RIGHT_OR_DOWN_LEFT(game->map->m[y + 1][x - 1].road))
                move_error = DOWN_LEFT_HAS_OBSTACLE;
            else if (game->map->m[y + 2][x - 2].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y + 2][x - 2].chess.player = prev_player;
                game->map->m[y + 2][x - 2].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {(y + 2) / 2, (x - 2) / 2};
            }
            break;
        case DOWN_RIGHT:
            if (y == game->map->h - 1 || x == game->map->w - 1)
                move_error = OUT_OF_DOWN_RIGHT_BOUND;
            else if (!IS_PATH(game->map->m[y + 1][x + 1].road) || !CAN_GO_UP_LEFT_OR_DOWN_RIGHT(game->map->m[y + 1][x + 1].road))
                move_error = DOWN_RIGHT_HAS_OBSTACLE;
            else if (game->map->m[y + 2][x + 2].chess.player != NULL_PLAYER)
                move_error = HAS_CHESS;
            else if (move_on_success)
            {
                game->map->m[y + 2][x + 2].chess.player = prev_player;
                game->map->m[y + 2][x + 2].chess.idx = prev_chess_idx;
                game->chess_records[PLAYER_IDX(prev_player)][prev_chess_idx] = (Point2D) {(y + 2) / 2, (x + 2) / 2};
            }
            break;
        default:
            move_error = UNKNOWN_OPERATION;
            break;
    }
    
    if (move_error == MOVE_SUCCESS && move_on_success) // 这里犯了把==写成=的错误
    {
        if (game->out_chess_moved)
            game->out_chess_moved = false;
        if (game->out_player == game->map->m[y][x].chess.player)
            game->out_chess_moved = true;
        game->map->m[y][x].chess.player = NULL_PLAYER;
    }

    return move_error;
}

bool move(Game *game, Point2D pos, Direction dir)
{
    MoveError move_error = move_try(game, pos, dir, true);
    print_move_error(move_error);
    return move_error == MOVE_SUCCESS;
}

static unsigned int get_movable_direction(Game *game, Point2D pos, Direction dir_arr[])
{
    unsigned int cnt = 0;

    for (Direction d = UP; d <= DOWN_RIGHT; d++)
        if (move_try(game, pos, d, false) == MOVE_SUCCESS)
            dir_arr[cnt++] = d;

    if (cnt != DIRECTION_NUM)
        dir_arr[cnt] = INVALID_DIRECTION;

    return cnt;
}

unsigned int get_available_move_choice(Game *game, Point2D movable_poses[], Direction movable_dirs[][DIRECTION_NUM], unsigned int dir_cnts[])
{
    unsigned int movable_chess_cnt = 0;

    for (ChessIdx chess_idx = 0; chess_idx < CHESS_PER_PLAYER; chess_idx++)
    {
        Player player_idx = PLAYER_IDX(game->round_player);
        Point2D pos = game->chess_records[player_idx][chess_idx];
        unsigned int movable_dir_cnt = get_movable_direction(game, pos, movable_dirs[movable_chess_cnt]);

        if (movable_dir_cnt)
        {
            movable_poses[movable_chess_cnt] = pos;
            dir_cnts[movable_chess_cnt] = movable_dir_cnt;
            movable_chess_cnt++;
        }
    }

    if (game->out_player != NULL_PLAYER && !game->out_chess_moved)
    {
        for (ChessIdx chess_idx = 0; chess_idx < CHESS_PER_PLAYER; chess_idx++)
        {
            Player player_idx = PLAYER_IDX(game->out_player);
            Point2D pos = game->chess_records[player_idx][chess_idx];
            unsigned int movable_dir_cnt = get_movable_direction(game, pos, movable_dirs[movable_chess_cnt]);

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

void player_lose(Game *game)
{
    for (Player k = FIRST_PLAYER; k <= LAST_PLAYER; k++)
        set_lose(game->status[PLAYER_IDX(k)]);

    char tmp_player = game->round_player;
    for (unsigned int i = 0; i < game->map->h; i += 2)
        for (unsigned int j = 0; j < game->map->w; j += 2)
            for (Player k = FIRST_PLAYER; k <= LAST_PLAYER; k++)
            {
                if (!is_out(game->status[PLAYER_IDX(k)]) && game->map->m[i][j].chess.player == k)
                {
                    Point2D pos = {i / 2, j / 2};
                    game->round_player = k;
                    for (Direction d = UP; d <= DOWN_RIGHT; d++)
                        if (move_try(game, pos, d, false) == MOVE_SUCCESS)
                        {
                            set_not_lose(game->status[PLAYER_IDX(k)]);
                            break;
                        }
                    break;
                }
            }
    game->round_player = tmp_player;

    for (Player k = FIRST_PLAYER; k <= LAST_PLAYER; k++)
        if (is_lose(game->status[PLAYER_IDX(k)]))
        {
            set_out(game->status[PLAYER_IDX(k)]);
            if (game->out_player == NULL_PLAYER)
                game->out_player = k;
        }

}
