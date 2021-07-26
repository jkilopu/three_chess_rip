/**
 * 建立所有可能的棋盘状态与自然数之间的双射
 *
 * 1. 这篇回答介绍了如何将组合序列映射到自然数:https://www.zhihu.com/question/51766178/answer/127412087
 *    但是我们的问题要复杂一些：将三种各三个棋子放在4*3的棋盘上。
 * 2. 种类一共有C(12, 3) * C(9, 3) * C(6, 3) * C(3, 3) 种，看到这个表达式我联想到了分步的做法，即一次每种棋子的摆放种数，得到3个idx
 *    这3个idx可以分别是三维数组的3个下标，但我这里直接把它映射到一维数组了（其实不用也行）。
 * 3. 这里将没有Player的位置（NULL_PLAYER）也算进去了，这样方便计算
 * 4. 注意下一步谁走是除棋子摆放方式外，棋盘状态的另一个组成要素。
 *
 * 代码参考：
 * 求组合数：https://www.tutorialspoint.com/binomial-coefficient-in-cplusplus
 *
 * 有点担心的方面：
 * 1. 没有错误检查
 * 2. 没有通用的接口（直接拿宏来用了）
 * 3. 状态很多（最多70,254,592种），不知道内存够不够用
 * 4. 只是实现了，细节还不太懂，要去看看上面的知乎文章
 */
#include "chess_idx.h"
#include "map.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MIN(_x, _y) ((_x < _y) ? (_x) : (_y))

static unsigned int *player_chess_possible_combination_nums;
static PlayerIdx player_num_r;

static unsigned int binom(unsigned int n, unsigned int m)
{
   unsigned int C[m + 1];
   memset(C, 0, sizeof(C));
   C[0] = 1;

   for (unsigned int i = 1; i <= n; i++) {
      for (unsigned int j = MIN(i, m); j != 0; j--)
         C[j] = C[j] + C[j - 1];
   }

   return C[m];
}

void create_and_init_combinations_state(PlayerArray *player_array, unsigned int map_real_size)
{
    player_num_r = player_array->player_num;
    player_chess_possible_combination_nums = malloc(sizeof(unsigned int) * player_array->player_num);

    for (PlayerIdx p_i = 0; p_i < player_array->player_num; p_i++)
    {
        player_chess_possible_combination_nums[p_i] = binom(map_real_size, player_array->players[p_i].chess_num);
        map_real_size -= player_array->players[p_i].chess_num;
    }
}

void destroy_combinations_state(void)
{
    free(player_chess_possible_combination_nums);
    player_chess_possible_combination_nums = NULL;
}

static unsigned int idx_of_single_player_combination(const PlayerIdx parr[], PlayerIdx current_player_idx, ChessIdx current_player_chess_num)
{
    unsigned int idx = 0;

    unsigned int j = 0;
    unsigned int k = 0;
    for (PlayerIdx i = 1; i <= current_player_chess_num; i++)
    {
        while (parr[k] != current_player_idx) // 如果carr中target的数量小于m，就会越界读
        {
            if (parr[k] > current_player_idx) // 跳过已计算idx以及NULL_PLAYER_IDX的Chess，不算入位数
                j++;
            k++;
        }
        idx += binom(j, i);
        k++;
        j++;
    }

    return idx;
}

unsigned int idx_of_players_combinations(const PlayerIdx parr[], const PlayerArray *player_array)
{
    unsigned int idx = 0;

    for (PlayerIdx current_player_idx = 0; current_player_idx < player_array->player_num; current_player_idx++) // 从NULL_PLAYER开始计算idx
    {
        unsigned int single_player_combination_idx = idx_of_single_player_combination(parr, current_player_idx, player_array->players[current_player_idx].chess_num);
        // 多个多维idx转为一个一维idx
        for (PlayerIdx p_i = current_player_idx + 1; p_i < player_array->player_num; p_i++)
            single_player_combination_idx *= player_chess_possible_combination_nums[p_i];
        idx += single_player_combination_idx;
    }

    return idx;
}

static void convert_2Dmap_to_1Dplayer_seq(const Map *map, PlayerIdx parr[])
{
    unsigned int cnt = 0;

    for (unsigned int y = 1; y < map->inner_h; y += 2)
        for (unsigned int x = 1; x < map->inner_w; x += 2)
            parr[cnt++] = map->m[y][x].chess_record.p_idx; // 包括NULL_PLAYER_IDX
}

unsigned int get_round_possible_states_num(void)
{
    unsigned int cnt = 1;

    for (PlayerIdx p_i = 0; p_i < player_num_r; p_i++)
        cnt *= player_chess_possible_combination_nums[p_i];
    cnt *= player_num_r * (player_num_r + 1) * 2;

    return cnt;
}

// [combination_idx][round_idx][out_idx][moved]
unsigned int idx_of_round(const Round *round)
{
    PlayerIdx parr[round->map.visable_h * round->map.visable_w];
    convert_2Dmap_to_1Dplayer_seq(&round->map, parr);

    PlayerIdx player_num = round->player_array.player_num;
    unsigned int player_combinations_idx = idx_of_players_combinations(parr, &round->player_array);

    return player_combinations_idx * player_num * (player_num + 1) * 2 +
        (round->round_player_idx + 1) * (player_num + 1) * 2 +
        ((round->out_player_idx == NULL_PLAYER_IDX ? player_num : round->out_player_idx) + 1) * 2 +
        round->out_chess_moved;
}
