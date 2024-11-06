#include "pwm.h"
#include "sam.h"
#include "utilities.h"

void pwm_init() {
    // Enable peripheral clock
    set_bit(PMC->PMC_PCER0, ID_PIOB);

    // Enable peripheral clock for PWM
    set_bit(PMC->PMC_PCER1, ID_PWM - 32);

    // Disable PIO on pin P12 and P13
    PIOB->PIO_PDR |= PIO_PDR_P12;
    PIOB->PIO_PDR |= PIO_PDR_P13;

    // Enable output on pin P12 and P13
    PIOB->PIO_OER |= PIO_OER_P12;
    PIOB->PIO_OER |= PIO_OER_P13;

    // Select peripheral B on pin P12 and P13
    PIOB->PIO_ABSR |= PIO_ABSR_P12;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;

    // Set prescaler to 1 for channel 1
    REG_PWM_CMR0 |= PWM_CMR_CPRE_MCK_DIV_16;
    REG_PWM_CMR1 |= PWM_CMR_CPRE_MCK_DIV_128;

    // Set Left Aligned
    REG_PWM_CMR0 &= ~PWM_CMR_CALG;
    REG_PWM_CMR1 &= ~PWM_CMR_CALG;

    // Set Clock Polarity
    REG_PWM_CMR0 |= PWM_CMR_CPOL;
    REG_PWM_CMR1 |= PWM_CMR_CPOL;

    // Set correct clock period - 20 ms (Assuming MCK = 84 MHz and prescaler = 1)
    REG_PWM_CPRD0 = CPRD * 8;
    REG_PWM_CPRD1 = CPRD;

    // Set initial duty cycle
    REG_PWM_CDTY0 = 0;
    REG_PWM_CDTY1 = DUTY_CYCLE_CENTRE;

    // PWM Channel Mode
    REG_PWM_ENA |= PWM_ENA_CHID0;
    REG_PWM_ENA |= PWM_ENA_CHID1;
}

void pwm_set_duty_cycle(uint32_t duty_cycle, uint32_t channel) {
    if (channel == 0) {
        REG_PWM_CDTY0 = duty_cycle;
    }
    else if (channel == 1) {
        if ((duty_cycle < DUTY_CYCLE_MAX) && (duty_cycle > DUTY_CYCLE_MIN))
        {
            REG_PWM_CDTY1 = duty_cycle;
        }
    }
}


