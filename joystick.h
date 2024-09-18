#pragma once
#include <stdlib.h>

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} joystick_direction;

typedef struct {
    int x;
    int y;
} joystick_position;