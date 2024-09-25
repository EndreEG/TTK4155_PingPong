#include "adc.h"

void adc_init(){
    set_bit(DDRD, DDD5); // Setter PD5 som output for ekstern klokke
    clear_bit(DDRD, DDD4); // Setter PD4 som input for ADC

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

uint8_t read_adc(uint8_t channel, uint8_t address) {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    ADC[channel] = address;
    return ADC[channel];
}

void adc_test(void) {
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
        uint8_t button_left = test_bit(PINB, PINB1);
        uint8_t button_right = test_bit(PINB, PINB0);
        uint8_t scaled_adc_x_value = (adc_x_value * 100) / 255;
        uint8_t scaled_adc_y_value = (adc_y_value * 100) / 255;
        slider_left = map_slider_output(slider_left);
        slider_right = map_slider_output(slider_right);
        // uint8_t scaled_slider_left = (slider_left * 100) / 255;
        // uint8_t scaled_slider_right = (slider_right * 100) / 255;
        // int16_t raw_adc = adc_value;
        // adc_value -= position_offset;
        printf("ADC value: (%d, %d),   (%d, %d), raw ADC: (%d, %d), Buttons: (%d, %d)", scaled_adc_x_value, scaled_adc_y_value, slider_left, slider_right, adc_x_value, adc_y_value, button_left, button_right);
        printf("\n\r"); 
        _delay_ms(200);
    }
}

uint8_t adc_read_channel(uint8_t channel) {
    volatile char *ADC = (char *) ADC_START_ADDRESS;
    if (channel == 0) {
        ADC[0] = 0;
        printf("Joystick x-value\n\r");
        uint8_t value = ADC[0];
        return value;
    }
    else if (channel == 1) {
        ADC[1] = 1;
        printf("Joystick y-value\n\r");
        uint8_t value = ADC[1];
        return value;
    }
    else if (channel == 2) {
        ADC[2] = 2;
        printf("Slider left\n\r");
        uint8_t value = ADC[2];
        return ADC[2];
    }
    else if (channel == 3) {
        ADC[3] = 3;
        printf("Slider right\n\r");
        uint8_t value = ADC[3];
        return value;
    }
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
