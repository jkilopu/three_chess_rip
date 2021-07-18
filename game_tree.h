#ifndef _GAME_TREE_H
#define _GAME_TREE_H

#include "game_def.h"

typedef unsigned int MovementIdx;

// 为了节省空间，很多东西都没有存在Node里，搜索时再计算
typedef struct _node{
    MovementIdx *mi;
    struct _node **next;
} Node;

typedef struct {
    Node *start_node;
} GameTree;

GameTree *create_game_tree(void);

#endif
