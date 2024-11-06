#pragma once

#include <stdint.h>

enum MotorDirection {
    LEFT,
    RIGHT
};

void motor_init();
void set_motor_direction(enum MotorDirection direction);
