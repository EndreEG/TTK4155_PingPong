#include "joystick.h"


/* joystick_direction get_joystick_direction(uint8_t x, uint8_t y) {
    int threshold = 512; // Example threshold value for neutral position

    if (pos.x < threshold - 100) {
        return LEFT;
    } else if (pos.x > threshold + 100) {
        return RIGHT;
    } else if (pos.y < threshold - 100) {
        return DOWN;
    } else if (pos.y > threshold + 100) {
        return UP;
    } else {
        return NEUTRAL;
    }
} */

uint8_t joystick_position_calibration() {
    joystick_position pos;
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