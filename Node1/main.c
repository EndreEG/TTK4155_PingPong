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
#include "menus.h"
#include "spi.h"
#include "mcp.h"
#include "can.h"


void main(void)
{
	uart_init(MYUBBR);
	adc_init();
	xmem_init();
	oled_init();
	can_init();
	printf("Mode: %x\n\r", mcp_read(MCP_CANSTAT));
	CanMessage message;
	can_construct_message(&message, 0x6, "BBAB");
	// can_transmit(&message);
	// _delay_ms(1);

	// while(1){	 can_transmit(&message);    }	

	while (1)
	{
		if (!can_receive(&message)) {
			printf("Waiting for message\n\r");
		}
		else {
			printf("Received message\n\r");
			can_print_message(&message);
			_delay_ms(1000);
		}
	}
	



	// CanMessage received_message = can_receive();
	// can_print_message(message);
	// free(message);
	// message = NULL;

	// while(1){
	// 	mcp_write(54, 0b10101010);
	// 	_delay_ms(100);
	// 	uint8_t result = mcp_read(54);
	// 	printf("Result: %d\n\r", result);
	// 	_delay_ms(100);

	// }
	// if (mcp_init_status) {
	// 	printf("MCP2515 initialization failed\n\r");
	// }
	// else {
	// 	printf("MCP2515 initialized successfully\n\r");
	// }
	// printf("Entering loop\n\r");
	// while (1)
	// {
	// mcp_write(0b00110110, 170);
	// uint8_t value = mcp_read(0b00110110);
	// printf("Value: %d\n\r", value);
	// }
	// oled_clear();
	// adc_test();
	// mcp_write(0x00, 'c');
	// printf("%d", mcp_read(0x00));
	// menu_init();
	// state current_state = MAIN_MENU;
	// while (1)
	// {
	// 	current_state = state_machine(current_state);
	// }
	
	
	// print_string("Hello World!", 3, 30, 0);
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