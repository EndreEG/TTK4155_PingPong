#pragma once

#include "joystick.h"
#include "utilities.h"
#include "adc.h"
#include "can.h"

void joystick_init() {
    clear_bit(DDRB, DDB2); // Set PB2 as input for joystick button (redundant)
    clear_bit(DDRB, DDB1); // Set PB1 as input for joystick button (redundant)
    clear_bit(DDRB, DDB0); // Set PB0 as input for joystick button (redundant)
    set_bit(PORTB, PB2); // Enable pull-up resistor on PB2
    clear_bit(PORTB, PB1); // Disable pull-up resistor on PB1
    clear_bit(PORTB, PB0); // Disable pull-up resistor on PB0
}

// uint8_t joystick_position_calibration() {
//     JoystickPosition pos;
//     uint32_t sum_x = 0;
//     uint32_t sum_y = 0;
//     uint16_t samples = 1000; // Does 1000 samples for calibration

//     for (uint16_t i = 0; i < samples; i++) {
//         sum_x += pos.x;
//         sum_y += pos.y;
//     }

//     uint16_t avg_x = sum_x / samples;
//     uint16_t avg_y = sum_y / samples;

//     // Store the calibration values in a global or static variable if needed
//     // calibrated_x = avg_x;
//     // calibrated_y = avg_y;

//     return (avg_x + avg_y) / 2; // Return the average calibration value
// }

JoystickPosition get_joystick_position() {
    JoystickPosition pos;
    uint8_t adc_readings[4];
    adc_read(&adc_readings);
    pos.x = adc_readings[0];
    pos.y = adc_readings[1];
    if (pos.x < 1) { 
        pos.x = 1;
    }

    return pos;
}

JoystickDirection get_joystick_direction(JoystickPosition pos) {
    if (pos.x < 130) {
        return LEFT;
    } else if (pos.x > 190) {
        return RIGHT;
    } else if (pos.y < 120) {
        return DOWN;
    } else if (pos.y > 220) {
        return UP;
    } else {
        return NEUTRAL;
    }
}

uint8_t is_joystick_button_pressed(){
    return !test_bit(PINB, PB2) || test_bit(PINB, PB1) || test_bit(PINB, PB0); // Returns 1 if any of the buttons are pressed.
} // PB2 is the joystick button, PB1 and PB0 are the capacitive buttons

void joystick_transmit(CanMessage* message, JoystickPosition* pos) {
    uint8_t position[2] = {pos->x, pos->y};
    can_construct_message(&message, JOYSTICK_CAN_ID, "BALL");
    // message.length = 3; // 2 bytes for position, 1 byte for direction
    // message.data[2] = dir;

    can_transmit(&message);
}

void BONK_BONK(JoystickPosition *pos, uint16_t midpoint_x, uint16_t midpoint_y) {
    // Fryser ved (50, 190), (0, 61), (166, 190), (53, 237) av en eller annen merkelig grunn

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
    if ((midpoint_y -3 < pos->y) && (pos->y < midpoint_y + 3)) { // Burde kanskje lage en kalibreringsfunksjon for y også
        pos->y = midpoint_y;
    }
}

void find_midpoint(uint16_t *midpoint_x, uint16_t *midpoint_y) {
    JoystickPosition pos;
    uint32_t sum_x = 0;
    uint32_t sum_y = 0;
    uint16_t samples = 9000; // Number of samples for calibration

    for (uint16_t i = 0; i < samples; i++) {
        pos = get_joystick_position();
        sum_x += pos.x;
        sum_y += pos.y;
        // sum_y += pos.y;
    }

    *midpoint_x = sum_x / samples;
    *midpoint_y = sum_y / samples;
}
