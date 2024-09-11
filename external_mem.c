#include "external_mem.h"

void sram_init() {
    MCUCR |= (1 << SRE); // Enable external memory on the MCU,
    SFIOR &= ~((111<<XMM0)); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet)
    SFIOR |= (1<<XMM2);

    // DDRE = (1 << DDE1);
}

void SRAM_test(void)
    {
        printf("Starter programmet\n\r");
        volatile char *SRAM = (char *) 0x1800;
        volatile char *ADC = (char *) 0x1400;
        while(1) {
            /*uint16_t i = 0;
            while (i < 100000)
            {
                i++;
            }
            i = 0; */
            SRAM[0] = 15;
            printf("Sram høy nå\n\r");
            _delay_ms(1000);
            /* while (i < 100000) {
                i++;
            }  
            i = 0;  */     
            ADC[0] = 15;        
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