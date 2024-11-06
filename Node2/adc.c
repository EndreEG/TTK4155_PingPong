#include "adc.h"
#include "sam.h"
#include "utilities.h"

void adc_init() {
    // Enable peripheral clock for ADC
    set_bit(PMC->PMC_PCER1, ID_ADC - 32);

    ADC->ADC_MR |= ADC_MR_FREERUN_ON; // Enable freerun mode
    ADC->ADC_MR |= ADC_MR_PRESCAL(49); // 84 MHz / (2 * (63 + 1)) = 0.5 MHz

    ADC->ADC_CHER = ADC_CHER_CH11; // Enable channel A11 - 11 er 9 in disguise.
    ADC->ADC_CR = ADC_CR_START; // Start ADC

    ADC->ADC_MR |= ADC_MR_TRANSFER(0);
    ADC->ADC_MR |= ADC_MR_TRACKTIM(2);
}

uint16_t adc_read() {
    uint16_t adc_value = ADC->ADC_CDR[11];
    return adc_value; // Read converted value
}
