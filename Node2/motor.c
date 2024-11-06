#include "motor.h"
#include "pwm.h"
#include "sam.h"
#include "utilities.h"


void motor_init() {
    // Enable peripheral clock
    set_bit(PMC->PMC_PCER0, ID_PIOC);

    // Enable output on pin P23
    PIOC->PIO_OER |= PIO_OER_P23;
    
    // Enable PIO on pin P23
    PIOC->PIO_PER |= PIO_PER_P23;

}

void set_motor_direction(enum MotorDirection direction) {
    switch (direction)
    {
    case LEFT:
        printf("LEFT\n\r");
        PIOC->PIO_SODR = PIO_PC23;
        break;
    case RIGHT:
        printf("RIGHT\n\r");
        PIOC->PIO_CODR = PIO_PC23;
        break;
    default:
        break;
    }
}