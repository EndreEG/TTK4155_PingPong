#include "joystick.h"

joystick_position get_joystick_position() {
    joystick_position pos;
    // Simulate reading analog values from hardware
    pos.x = rand() % 1024; // Replace with actual hardware reading
    pos.y = rand() % 1024; // Replace with actual hardware reading
    return pos;
}

joystick_direction get_joystick_direction() {
    joystick_position pos = get_joystick_position();
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
}