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
#include "chess_bijection.h"
#include "map_def.h"
#include <string.h>
#include <stdbool.h>

#define MIN(_x, _y) ((_x < _y) ? (_x) : (_y))

static unsigned int player_chess_possible_combination_num[PLAYER_NUM + 1]; // 包括NULL_PLAYER
bool state_inited = false;

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

static void init_state(unsigned int map_size)
{
    if (!state_inited)
    {
        for (Player p = NULL_PLAYER; p <= LAST_PLAYER; p++)
        {
            player_chess_possible_combination_num[p] = binom(map_size, CHESS_PER_PLAYER);
            map_size -= CHESS_PER_PLAYER;
        }
        // assert(map_size == 0); // 或者搞个Error类型
        state_inited = true;
    }
}

static unsigned int idx_of_single_player_combination(const Player parr[], Player current_player)
{
    unsigned int current_player_idx = 0;

    for (unsigned int i = 1, j = 0, k = 0; i <= CHESS_PER_PLAYER; i++)
    {
        while (parr[k] != current_player) // 如果carr中target的数量小于m，就会越界读
        {
            if (parr[k] > current_player) // 跳过已计算idx的Chess，不算入位数
                j++;
            k++;
        }
        current_player_idx += binom(j, i);
        k++;
        j++;
    }

    return current_player_idx;
}

static unsigned int idx_of_players_combinations(const Player parr[])
{
    unsigned int idx = 0;

    for (Player current_player = NULL_PLAYER; current_player <= LAST_PLAYER; current_player++) // 从NULL_PLAYER开始计算idx
    {
        unsigned int single_player_idx = idx_of_single_player_combination(parr, current_player);
        // 多个多维idx转为一个一维idx
        for (Player p = current_player + 1; p <= LAST_PLAYER; p++)
            single_player_idx *= player_chess_possible_combination_num[p];
        idx += single_player_idx;
    }

    return idx;
}

static void convert_2Dmap_to_1Dplayer_seq(const Map *map, Player parr[])
{
    unsigned int cnt = 0;

    for (unsigned int i = 0; i < map->h; i += 2)
        for (unsigned int j = 0; j < map->w; j += 2)
            parr[cnt++] = map->m[i][j].chess.player; // 包括NULL_PLAYER
}

unsigned int idx_of_map(const Map *map, Player round_player)
{
    init_state((map->w / 2) * (map->h / 2));

    Player parr[(map->w / 2) * (map->h / 2)];
    convert_2Dmap_to_1Dplayer_seq(map, parr);

    return idx_of_players_combinations(parr) * (PLAYER_IDX(round_player) + 1);
}
