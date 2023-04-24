#ifndef _GAME_TREE_DEF_H
#define _GAME_TREE_DEF_H

#include "movement_idx_def.h"
#include <stdbool.h>

typedef unsigned char KidIdx;
typedef unsigned long long ResultIdx;
typedef unsigned char NodeStatus;

enum {
    COMPLETE_WIN = 1,
    NOT_COMPLETE_WIN,
    UNDETERMINED,
};

// 为了节省空间，很多东西都没有存在Node里，搜索时再计算
typedef struct _node{
    MovementIdx *mi;
    struct _node **kids;
    ResultIdx win_num;
    ResultIdx total_num;
    NodeStatus node_status;
    KidIdx kid_num;
} Node;

typedef struct {
    Node *start_node;
    Node *cur_node;
} GameTree;

#endif
