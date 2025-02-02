#include "sram.h"

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
    volatile char *ext_ram = ( char * ) SRAM_START_ADDRESS;
    ext_ram[address] = data;  
}

uint8_t read_sram(uint16_t address) {
    volatile char *ext_ram = ( char * ) SRAM_START_ADDRESS;
    // printf(ext_ram[address]);
    return ext_ram[address];
}

void sram_test(void) {
    volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
    uint16_t ext_ram_size = 0x800;
    uint16_t write_errors = 0;
    uint16_t retrieval_errors = 0;
    printf("Starting SRAM test...\r\n");
    // rand() stores some internal state, so calling this function in a loop will
    // yield different seeds each time (unless srand() is called before this function)
    uint16_t seed = rand();
    // Write phase: Immediately check that the correct value was stored
    srand(seed);
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        ext_ram[i] = some_value;
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            write_errors++;
        }
    }
    // Retrieval phase: Check that no values were changed during or after the write phase
    srand(seed);
    // reset the PRNG to the state it had before the write phase
    for (uint16_t i = 0; i < ext_ram_size; i++) {
        uint8_t some_value = rand();
        uint8_t retreived_value = ext_ram[i];
        if (retreived_value != some_value) {
            printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
            retrieval_errors++;
        }
    }
    printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}