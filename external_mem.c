#include "external_mem.h"

void sram_init() {
    MCUCR |= (1 << SRE); // Enable external memory on the MCU,
    SFIOR &= ~((111<<XMM0)); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet)
    SFIOR |= (1<<XMM2);

    // DDRE = (1 << DDE1);
}

void SRAM_test(void)
    {
		volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
        uint16_t ext_ram_size = 0x800;
        printf("Starting SRAM test...\n");
        // rand() stores some internal state, so calling this function in a loop will
        // yield different seeds each time (unless srand() is called before this function)
        // Write phase: Immediately check that the correct value was stored
        // for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = 5;
        ext_ram[0] = some_value;
        uint8_t retreived_value = ext_ram[0];
        printf("%d, %d\n\r", some_value, retreived_value);
        //}
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