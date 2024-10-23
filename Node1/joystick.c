#pragma once

#include "joystick.h"
#include "adc.h"
#include "can.h"


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


void joystick_transmit(JoystickPosition pos, JoystickDirection dir) {
    // Create a CAN message and transmit it
    CanMessage *message = malloc(sizeof(CanMessage));
    can_construct_message(message, JOYSTICK_CAN_ID, (uint8_t*)&pos);
    can_transmit(message);
    free(message);
    message = NULL;
}