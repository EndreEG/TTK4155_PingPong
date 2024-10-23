#include "adc.h"

void adc_init(){
    set_bit(DDRD, DDD5); // Setter PD5 som output for ekstern klokke
    clear_bit(DDRD, DDD4); // Setter PD4 som input for busy signal
    clear_bit(DDRB, DDB7); // Setter PB7 som input joystick button pressed
    set_bit(PORTB, PORTB7); // Setter pull-up resistor på PB7

	//set fast PWM mode, choose ICR1 as TOP
	set_bit(TCCR1B,WGM13);
	set_bit(TCCR1B,WGM12);
	set_bit(TCCR1A,WGM11);
	clear_bit(TCCR1A,WGM10);
	// clear on compare match, set on TOP reached
	set_bit(TCCR1A,COM1A1);
	clear_bit(TCCR1A,COM1A0);

	// sets I/O clock as frequency, with no prescaler
	set_bit(TCCR1B,CS10);
	clear_bit(TCCR1B,CS11);
	clear_bit(TCCR1B,CS12);
    OCR1A = 1;
    ICR1 = 3;

    clear_bit(DDRB, DDB0);
    clear_bit(DDRB, DDB1);
}

void adc_test(void) {
    printf("ADC test start\n\r");
    uint8_t* adc_readings = (uint8_t*) malloc(4);
    while(1) {
        adc_read(adc_readings);
        printf("ADC values: (%d, %d), (%d, %d)\n\r", adc_readings[0], adc_readings[1], adc_readings[2], adc_readings[3]);
        _delay_ms(100);
    }
    free(adc_readings);
    adc_readings = NULL;
}

void adc_read(uint8_t* adc_readings) {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    ADC[0] = 0;
    _delay_us(1);
    loop_until_bit_is_set(PIND, PIND4);
    adc_readings[1] = ADC[0]; // X-axis
    adc_readings[0] = ADC[1]; // Y-axis
    adc_readings[2] = ADC[2]; // Left slider
    adc_readings[3] = ADC[3]; // Right slider
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
