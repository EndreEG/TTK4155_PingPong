#include "decoder.h"

void decoder_init() {
    REG_PMC_PCER1 |= ID_TC6 - 32;

    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;

    PIOC->PIO_ABSR |= PIO_ABSR_P25;
    PIOC->PIO_ABSR |= PIO_ABSR_P26;

    REG_TC2_CMR0 = TC_CMR_TCCLKS_XC0;

    REG_TC2_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;

    REG_TC2_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

uint16_t decoder_read() {
    return REG_TC2_CV0;
}