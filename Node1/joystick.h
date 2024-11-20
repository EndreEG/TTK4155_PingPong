#pragma once

#define JOYSTICK_CAN_ID 0x10
#define MIDPOINT_X 168
#define MIDPOINT_Y 172

#include "utilities.h"
#include "can.h"


// Left slider : PB5
// Right slider : PD2
// Left button : PB6
// Right button : PD1
// Filter cut-off frequency: 795.8 Hz

// Slider voltage: [0.09, 0.5, 1.0, 1.49, 2.02, 2.52, 3.0, 3.51, 4.0, 4.5, 5.0]
// Slider values [5, 33, 65, 100, 133, 166, 190, 225, 252, 255]

typedef enum {
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NEUTRAL
} JoystickDirection;

typedef struct {
    int x;
    int y;
} JoystickPosition;

void joystick_init();
JoystickDirection get_joystick_direction(JoystickPosition pos);
JoystickPosition get_joystick_position();
uint8_t is_joystick_button_pressed();
// uint8_t joystick_position_calibration();
void joystick_transmit(CanMessage * message, JoystickPosition *pos);
void BONK_BONK(JoystickPosition *pos, uint16_t midpoint_x, uint16_t midpoint_y);
void find_midpoint(uint16_t *midpoint_x, uint16_t *midpoint_y);