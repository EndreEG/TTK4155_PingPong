#include "utilities.h"

void xmem_init() {
    set_bit(MCUCR, SRE); // Enable external memory on the MCU
    clear_bit(SFIOR, XMM0); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet), ensure that the XMM-pins are initialised to zero.
    clear_bit(SFIOR, XMM1);
    clear_bit(SFIOR, XMM2);
    set_bit(SFIOR, XMM2); // Release pin 4-7 for the JTAG

    set_bit(SREG, 7);
}

int get_font8_index(char c) {
    if (c >= ' ' && c <= '~') {
        return c - ' ';
    }
    return -1; // Return -1 for characters outside the supported range
}