#include "decoder.h"
#include "utilities.h"

void decoder_init() {
    // PMC->PMC_WPMR = 0x504D4300;
    // PIOC->PIO_WPMR = 0x50494F00;
    // REG_PMC_PCER1 |= (1 << ID_TC6 - 32);
    set_bit(PMC->PMC_PCER1, ID_TC6 - 32);
    
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;

    PIOC->PIO_OER |= PIO_OER_P25;
    PIOC->PIO_OER |= PIO_OER_P26;

    PIOC->PIO_ABSR |= PIO_ABSR_P25;
    PIOC->PIO_ABSR |= PIO_ABSR_P26;

    // REG_TC2_WPMR = 0x54494D00;
    // TC2->TC_WPMR = 0x54494D00;
    TC2->TC_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    // REG_TC2_CMR0 = TC_CMR_TCCLKS_XC0;

    // REG_TC2_BMR = TC_BMR_QDEN | TC_BMR_POSEN | TC_BMR_EDGPHA;

    // REG_TC2_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

float decoder_read() {
    if (REG_TC2_CV0 > MAX_DECODER_VALUE) {
        return MAX_DECODER_VALUE;
    }
    else if (REG_TC2_CV0 < MIN_DECODER_VALUE) {
        return MIN_DECODER_VALUE;
    }
    return REG_TC2_CV0 / MAX_DECODER_VALUE;
}