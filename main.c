/*
 * main.c
 *
 * Created: 9/4/2024 3:15:02 PM
 *  Author: endreeg
 */ 
#include "uart.h"

void main(void)
{
	DDRA = (1<<PA0);

    uart_init(MYUBBR);

	PORTA = (1<<PA0);
	printf("Ting funker\n\r");

}