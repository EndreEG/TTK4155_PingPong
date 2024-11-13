#pragma once

#define MIDPOINT_X 168

#include <stdint.h>

void set_servo_pos(uint8_t data[8]);
void set_motor_pos(uint8_t data[8]);
void solenoid_init();
void fire_solenoid();
void PI_controller_init();
void PI_controller(float _actual_pos, float _ref_pos, uint8_t direction);
uint16_t ref_pos;
uint16_t actual_pos;
float error;
// uint16_t error_sum;
uint8_t execute_controller;
bool should_execute_controller();
void set_reference_position(uint8_t pos);