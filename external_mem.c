#include "external_mem.h"

void sram_init() {
    MCUCR |= (1 << SRE); // Enable external memory on the MCU
    SFIOR &= ~((111<<XMM0)); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet), ensure that the XMM-pins are initialised to zero.
    SFIOR |= (1<<XMM2); // Release pin 4-7 for the JTAG
}

void chip_select_test(void)
    {
        printf("Starter programmet\n\r");
        volatile char *SRAM = (char *) SRAM_START_ADDRESS; // SRAM sitt adresserom
        volatile char *ADC = (char *) ADC_START_ADDRESS; // ADC-en sitt adresserom
        while(1) {
            SRAM[0] = 15; // Tester chip select signalet ved å sette adresse 0x1800 høy
            printf("Sram høy nå\n\r");
            _delay_ms(1000);   
            ADC[0] = 15; // Tester chip select ved å sette ADC-en (adresse 0x1400 i dette tilfellet) høy, og da skal SRAM gå lav
            printf("ADC høy nå\n\r");
            _delay_ms(1000);
        }
}

void write_sram(uint16_t address, uint8_t data) {
    volatile char *ext_ram = ( char * ) OLED_START_ADDRESS;
    ext_ram[address] = data;  
}

uint8_t read_sram(uint16_t address) {
    volatile char *ext_ram = ( char * ) OLED_START_ADDRESS;
    // printf(ext_ram[address]);
    return ext_ram[address];
}