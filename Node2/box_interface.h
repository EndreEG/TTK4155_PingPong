#pragma once

#define MIDPOINT_X 168

#include <stdint.h>

void set_servo_pos(uint8_t data[8]);
void set_motor_pos(uint8_t data[8]);
void fire_solenoid();