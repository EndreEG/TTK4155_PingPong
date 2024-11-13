#include <stdlib.h>
#include "sam.h"
#include "time.h"
#include "utilities.h"
#include "box_interface.h"
#include "motor.h"
#include "pwm.h"

void set_motor_pos(uint8_t data[8]) {
    uint8_t x_pos = data[0];
    uint16_t midpoint_x = data[3];
    uint32_t duty_cycle = 100 * abs(x_pos - midpoint_x);
    printf("x pos: %d, midpoint: %d, duty cycle: %d\n\r", x_pos, midpoint_x, duty_cycle);
    set_motor_direction((enum MotorDirection)data[2]);
    pwm_set_duty_cycle(duty_cycle, 0);
}

void set_servo_pos(uint8_t data[8]) {
    uint8_t y_pos = data[1];
    uint32_t duty_cycle = DUTY_CYCLE_MIN + ((DUTY_CYCLE_MAX - DUTY_CYCLE_MIN) / 255) * y_pos;
    // printf("Setting duty cycle to %d\n\r", duty_cycle);
    pwm_set_duty_cycle(duty_cycle, 1);
}

void solenoid_init() {
    // Enable peripheral clock
    set_bit(PMC->PMC_PCER0, ID_PIOA);
    PIOA->PIO_PER  = PIO_PA10;
    PIOA->PIO_OER  = PIO_PA10;
}

void fire_solenoid() {
    PIOA->PIO_CODR = PIO_PA10;
    time_spinFor(msecs(24));
    PIOA->PIO_SODR = PIO_PA10;
}

void PI_controller_init() {
    // Set up PI controller using timer counter interrupt
    NVIC_EnableIRQ(TC0_IRQn); // Enable interrupt
    set_bit(PMC->PMC_PCER0, ID_TC0); // Enable peripheral clock
    
    // Set up timer counter
    REG_TC0_CMR0 = TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC;
    REG_TC0_RC0 = 1000;

    REG_TC0_IER0 = TC_IER_CPCS;
    // REG_TC0_IMR0 = TC_IMR_CPCS;

    REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
    
    ref_pos = 0;
    actual_pos = 0;
    error = 0;
    error_sum = 0;
}

void TC0_Handler(void) {
    // Clear the interrupt flag
    REG_TC0_SR0;
    execute_controller = 1;
}

bool should_execute_controller() {
    return execute_controller;
}

void set_reference_position(uint8_t pos) {
    ref_pos = pos;
}

void PI_controller(uint32_t _actual_pos, uint8_t _ref_pos, uint8_t direction) {
    execute_controller = 0;
    error = _ref_pos - _actual_pos;
    error_sum += error;
    uint32_t duty_cycle = 0.1 * error + 0.01 * error_sum;
    set_motor_direction((enum MotorDirection)direction);
    pwm_set_duty_cycle(duty_cycle, 0);
}
