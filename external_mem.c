#include "external_mem.h"

void sram_init() {
    MCUCR |= (1 << SRE); // Enable external memory on the MCU
    SFIOR &= ~((111<<XMM0)); // Release JTAG ports from address bus (s. 32 ATMega162 datasheet), ensure that the XMM-pins are initialised to zero.
    SFIOR |= (1<<XMM2); // Release pin 4-7 for the JTAG
}

void adc_init(){
    DDRD |= (1 << DDD4); // Setter PD5 som output for ekstern klokke
    TCCR3A |= (0 << COM3A0) | (1 << COM3A1) | (1 << WGM31) | (1 << WGM30);
    TCCR3B |= (0 << ICES3) | (1 << WGM32) | (1 << CS30); // Setter CTC som Timer/Counter mode of operation
    OCR3B = 0;
    //TCCR3B |= (1 << CS30); // Setter clock prescaler til 1/ Ingen prescaling
    //TCCR3A |= (1 << COM3A1); // Setter COM1A0 til 1
    //OCR3B = 0;
}

uint8_t read_adc(uint8_t channel, uint8_t address) {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    ADC[channel] = address;
    return ADC[channel];
}

int8_t calibration() {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    uint8_t calibration_index = 0;
    uint16_t calibration_sum = 0;
    while(calibration_index < 10) {
        ADC[0] = 0;
        _delay_us(100);
        // printf("Current sum %d, current index %d\n\r", calibration_sum, calibration_index);
        uint8_t adc_value = ADC[0];
        calibration_sum = calibration_sum + adc_value;
        printf("ADC value %d, calibration sum %d\n\r", adc_value, calibration_sum);
        calibration_index++;
    }
    uint8_t position_offset = calibration_sum / 10;
    printf("Offset: %d\n\r", position_offset);
    return position_offset;
        // printf("Position offset after %d\n\r", position_offset);
}

uint8_t map_slider_output(uint8_t slider_value) {
    uint8_t new_min = 0;
    uint8_t new_max = 100;
    uint8_t old_min = 5;
    uint8_t old_max = 255;
    return (new_max - new_min) * (slider_value - old_min) / (old_max - old_min) + new_min;
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
    // int32_t position_offset = calibration();
    // printf("Pos offset: %d\n\r", position_offset);
    // int16_t adc_value;
    while(1) {
        ADC[0] = 0;
        ADC[1] = 1;
        ADC[2] = 2;
        ADC[3] = 3;
        _delay_ms(10);
        uint8_t adc_x_value = ADC[0];
        uint8_t adc_y_value = ADC[1];
        uint8_t slider_left = ADC[2];
        uint8_t slider_right = ADC[3];
        uint8_t scaled_adc_x_value = (adc_x_value * 100) / 255;
        uint8_t scaled_adc_y_value = (adc_y_value * 100) / 255;
        slider_left = map_slider_output(slider_left);
        slider_right = map_slider_output(slider_right);
        // uint8_t scaled_slider_left = (slider_left * 100) / 255;
        // uint8_t scaled_slider_right = (slider_right * 100) / 255;
        // int16_t raw_adc = adc_value;
        // adc_value -= position_offset;
        printf("ADC value: (%d, %d),   (%d, %d), raw ADC: ", scaled_adc_x_value, scaled_adc_y_value, slider_left, slider_right );
        printf("\n\r"); 
        _delay_ms(200);
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