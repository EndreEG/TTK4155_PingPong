#include "external_mem.h"

void sram_init() {
    MCUCR |= (1 << SRE); // Enable external memory on the MCU
    SFIOR &= ~((111<<XMM0)); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet), ensure that the XMM-pins are initialised to zero.
    SFIOR |= (1<<XMM2); // Release pin 4-7 for the JTAG
}

void adc_init(){
    DDRD |= (1<<PD5);
}

uint8_t read_adc(uint8_t channel, uint8_t address) {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    ADC[channel] = address;
    return ADC[channel];
}

int8_t calibration() {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    uint8_t calibration_index = 0;
    int16_t calibration_sum = 0;
    while(calibration_index < 10) {
        ADC[calibration_index] = 0x0;
        _delay_us(100);
        // printf("Current sum %d, current index %d\n\r", calibration_sum, calibration_index);
        int16_t adc_value = ADC[calibration_index];
        calibration_sum = calibration_sum + adc_value;
        printf("ADC value %d, calibration sum %d\n\r", adc_value, calibration_sum);
        calibration_index++;
    }
    int8_t position_offset = calibration_sum / 10;
    printf("Offset: %d\n\r", position_offset);
    return position_offset;
        // printf("Position offset after %d\n\r", position_offset);
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

void ADC_test(void) {
    printf("ADC test start\n\r");
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    int32_t index = 0;
    int32_t position_offset = calibration();
    printf("Pos offset: %d\n\r", position_offset);
    int16_t adc_value = 0x0;
    while(1) {
        // printf("First iteration sum: %d, offset: %d\n\r", calibration_sum, position_offset);
        ADC[index] = 0x0;
        _delay_us(20);
        adc_value = ADC[index];
        printf("Index %d: %d\n\r", index, adc_value);
        //printf("Index 1: %d\n\r", ADC[1]);
        printf("\n\r");
        _delay_ms(200);
        index++;
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