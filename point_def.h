#ifndef _POINT_DEF_H
#define _POINT_DEF_H

typedef struct {
    unsigned int y, x;
} Point2D;
typedef struct {
    Point2D first;
    Point2D second;
} PairPoint2D;

#endif
