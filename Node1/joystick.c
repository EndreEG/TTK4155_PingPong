#pragma once

#include "joystick.h"
#include "adc.h"
#include "can.h"

void joystick_init() {
    clear_bit(DDRB, DDB2); // Set PB2 as input for joystick button (redundant)
    set_bit(PORTB, PB2); // Enable pull-up resistor on PB2
}

uint8_t joystick_position_calibration() {
    JoystickPosition pos;
    uint32_t sum_x = 0;
    uint32_t sum_y = 0;
    uint16_t samples = 1000; // Number of samples for calibration

    for (uint16_t i = 0; i < samples; i++) {
        sum_x += pos.x;
        sum_y += pos.y;
    }

    uint16_t avg_x = sum_x / samples;
    uint16_t avg_y = sum_y / samples;

    // Store the calibration values in a global or static variable if needed
    // calibrated_x = avg_x;
    // calibrated_y = avg_y;

    return (avg_x + avg_y) / 2; // Return the average calibration value
}

JoystickPosition get_joystick_position() {
    JoystickPosition pos;
    uint8_t adc_readings[4];
    adc_read(&adc_readings);
    pos.x = adc_readings[0];
    if (pos.x < 1) {
        pos.x = 1;
    }

    pos.y = adc_readings[1];
    return pos;
}

JoystickDirection get_joystick_direction(JoystickPosition pos) {
    if (pos.x < 130) {
        return LEFT;
    } else if (pos.x > 190) {
        return RIGHT;
    } else if (pos.y < 130) {
        return DOWN;
    } else if (pos.y > 190) {
        return UP;
    } else {
        return NEUTRAL;
    }
}

uint8_t is_joystick_button_pressed(){
    return !test_bit(PINB, PB2);
}

void joystick_transmit(CanMessage* message, JoystickPosition* pos) {
    uint8_t position[2] = {pos->x, pos->y};
    can_construct_message(&message, JOYSTICK_CAN_ID, "BALL");
    // message.length = 3; // 2 bytes for position, 1 byte for direction
    // message.data[2] = dir;

    can_transmit(&message);
}

void BONK_BONK(JoystickPosition *pos, uint16_t midpoint_x) {
    // Fryser ved (50, 190), (0, 61), (166, 190), (53, 237)

    if (pos->x == 0 && pos->y == 61) {
        pos->x = 0;
        pos->y = 62;
    }
    else if (pos->x == 50 && pos->y == 190) {
        pos->x = 50;
        pos->y = 191;
    }
    else if (pos->x == 166 && pos->y == 190) {
        pos->x = 166;
        pos->y = 191;
    }
    else if (pos->x == 53 && pos->y == 237) {
        pos->x = 53;
        pos->y = 238;
    }
    else if (pos->x == 169 && pos->y == 6) {
        pos->x = 168;
        pos->y = 6;
    }
    // printf("X: %d\n\r", pos->x);
    if ((midpoint_x -3 < pos->x) && (pos->x < midpoint_x + 3)) {
        pos->x = midpoint_x;
    }
    if ((MIDPOINT_Y -3 < pos->y) && (pos->y < MIDPOINT_Y + 3)) {
        pos->y = MIDPOINT_Y;
    }
}

uint16_t find_midpoint() {
    JoystickPosition pos;
    uint32_t sum_x = 0;
    // uint32_t sum_y = 0;
    uint16_t samples = 9000; // Number of samples for calibration

    for (uint16_t i = 0; i < samples; i++) {
        pos = get_joystick_position();
        sum_x += pos.x;
        // sum_y += pos.y;
    }

    uint16_t avg_x = sum_x / samples;
    // uint16_t avg_y = sum_y / samples;

    return avg_x; // Return the average calibration value
}
