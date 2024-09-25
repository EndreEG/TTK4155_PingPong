#include "utilities.h"

void xmem_init() {
    MCUCR |= (1 << SRE); // Enable external memory on the MCU
    SFIOR &= ~((111<<XMM0)); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet), ensure that the XMM-pins are initialised to zero.
    SFIOR |= (1<<XMM2); // Release pin 4-7 for the JTAG
}

int get_font8_index(char c) {
    if (c >= ' ' && c <= '~') {
        return c - ' ';
    }
    return -1; // Return -1 for characters outside the supported range
}