#include "error.h"
#include <stdio.h>
#include <stdlib.h>

static char *move_err_dir_str[] = {
    "up",
    "down",
    "left",
    "right",
    "up-left",
    "up-right",
    "down-left",
    "down-right",
};

void print_move_error(MoveError move_error)
{
    switch (move_error)
    {
        case UNKNOWN_OPERATION:
            puts("MoveError: Unknown move operation.");
            break;
        case MOVE_SUCCESS:
            break;
        case POS_OUT_OF_BOUND:
            puts("MoveError: Pos out of bound.");
            break;
        case WRONG_PLAYER:
            puts("MoveError: Wrong player.");
            break;
        case DOUBLE_CHESS_MOVED_LAST_TIME:
            puts("MoveError: Double chess moved last time.");
            break;
        case NOT_DOUBLE_CHESS:
            puts("MoveError: Selected chess is not a double chess.");
            break;
        case HAS_CHESS:
            puts("MoveError: Destination has chess.");
            break;
        case UP_HAS_OBSTACLE:
        case DOWN_HAS_OBSTACLE:
        case LEFT_HAS_OBSTACLE:
        case RIGHT_HAS_OBSTACLE:
        case UP_LEFT_HAS_OBSTACLE:
        case UP_RIGHT_HAS_OBSTACLE:
        case DOWN_LEFT_HAS_OBSTACLE:
        case DOWN_RIGHT_HAS_OBSTACLE:
            printf("MoveError: Has obstacle at the %s direction.\n", move_err_dir_str[move_error - UP_HAS_OBSTACLE]);
            break;
        case OUT_OF_UP_BOUND:
        case OUT_OF_DOWN_BOUND:
        case OUT_OF_LEFT_BOUND:
        case OUT_OF_RIGHT_BOUND:
        case OUT_OF_UP_LEFT_BOUND:
        case OUT_OF_UP_RIGHT_BOUND:
        case OUT_OF_DOWN_LEFT_BOUND:
        case OUT_OF_DOWN_RIGHT_BOUND:
            printf("MoveError: Move out of %s bound.\n", move_err_dir_str[move_error - OUT_OF_UP_BOUND]);
            break;
        default:
            puts("MoveError: Unknown move error. Exit.");
            exit(1);
    }
}

void print_robot_error(RobotError robot_error)
{
    switch (robot_error)
    {
        case NO_AVAILABLE_CHOICE:
            puts("RobotError: No available choice for robot.");
            exit(1);
        case ROBOT_SUCCESS:
            break;
        default:
            puts("RobotError: Unknown robot error. Exit.");
            exit(1);
    }
}
