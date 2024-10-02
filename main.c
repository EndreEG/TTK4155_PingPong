/*
 * main.c
 *
 * Created: 9/4/2024 3:15:02 PM
 *  Author: endreeg
 */ 
#include "uart.h"
#include "adc.h"
#include "sram.h"
#include "oled.h"


void main(void)
{
	//adc_init();
	uart_init(MYUBBR);
	adc_init();
	xmem_init();
	oled_init();
	oled_clear();
	menu();
	uint8_t current_arrow_pos = 2;
	uint8_t previous_arrow_pos = 2;
	uint8_t *adc_readings = (uint8_t *) malloc(4);
	joystick_direction direction = NEUTRAL;
	oled_print_arrow(current_arrow_pos, 0);
	while (1)
	{
		adc_read(adc_readings);
		direction = get_joystick_direction(adc_readings[0], adc_readings[1]);
		current_arrow_pos = move_arrow(current_arrow_pos, direction);
		printf("Current arrow pos: %d Previous arrow pos: %d Direction: %d\n\r", current_arrow_pos, previous_arrow_pos, direction);
		if (current_arrow_pos != previous_arrow_pos) {
			previous_arrow_pos = current_arrow_pos;
			_delay_ms(200);
		}
	}
	free(adc_readings);
	adc_readings = NULL;
	
	// print_string("Hello World!");
	// adc_test();

	// sram_test();
	// chip_select_test();
	// while(1) {
	// 	volatile char *ADC = (char *) ADC_START_ADDRESS;
	// 	ADC[0] = 1;
	// 	_delay_ms(10);
	// 	uint8_t value = ADC[0];
	// 	printf("ADC-verdi: %d\n\n\r", value);
	// 	_delay_ms(50);
	// }
}