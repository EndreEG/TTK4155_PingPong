/*
 * main.c
 *
 * Created: 9/4/2024 3:15:02 PM
 *  Author: endreeg
 */ 
#include "uart.h"
#include "sram.h"
#include "sram_test.h"
#include "oled.h"


void main(void)
{
	//adc_init();
	uart_init(MYUBBR);
	adc_init();
	xmem_init();
	oled_init();
	oled_clear();
	print_string("Hello World!");
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