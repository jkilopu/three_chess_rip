#ifndef _ROAD_H
#define _ROAD_H

#include "road_def.h"

#define PATH_BIT (0)
#define GO_UP_RIGHT_OR_DOWN_LEFT_BIT (1)
#define GO_UP_LEFT_OR_DOWN_RIGHT_BIT (2)

#define is_path(road) ((1 << PATH_BIT) & road)
#define set_path(road) (road |= (1 << PATH_BIT))

#define can_go_up_right_or_down_left(road) ((1 << GO_UP_RIGHT_OR_DOWN_LEFT_BIT) & road)
#define set_go_up_right_or_down_left(road) (road |= (1 << GO_UP_RIGHT_OR_DOWN_LEFT_BIT))
#define can_go_up_left_or_down_right(road) ((1 << GO_UP_LEFT_OR_DOWN_RIGHT_BIT) & road)
#define set_go_up_left_or_down_right(road) (road |= (1 << GO_UP_LEFT_OR_DOWN_RIGHT_BIT))


#endif
