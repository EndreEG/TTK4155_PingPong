#include "box_interface.h"
#include "motor.h"
#include "pwm.h"
#include <stdlib.h>

void set_motor_pos(uint8_t data[8]) {
    uint8_t x_pos = data[0];
    uint32_t duty_cycle = 100 * abs(x_pos - MIDPOINT_X);
    pwm_set_duty_cycle(duty_cycle, 0);
    set_motor_direction((enum MotorDirection)data[2]);
}

void set_servo_pos(uint8_t data[8]) {
    uint8_t y_pos = data[1];
    uint32_t duty_cycle = DUTY_CYCLE_MIN + ((DUTY_CYCLE_MAX - DUTY_CYCLE_MIN) / 255) * y_pos;
    printf("Setting duty cycle to %d\n\r", duty_cycle);
    pwm_set_duty_cycle(duty_cycle, 1);
}


void fire_solenoid() {
    
}