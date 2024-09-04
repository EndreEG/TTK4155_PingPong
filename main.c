/*
 * main.c
 *
 * Created: 9/4/2024 3:15:02 PM
 *  Author: endreeg
 */ 

#include <stdio.h>
#include <xc.h>
#include <avr/io.h>

#define FOSC 4915200 // Crystal oscillator frequency
#define BAUDRATE 9600
#define MYUBBR FOSC/16/BAUDRATE - 1
//#define F_CPU 16000000
//#include "util/delay.h"

int uart_recieve(FILE * file);
int uart_transmit(char data, FILE * file);

void uart_init( unsigned int ubrr )
{
	/* Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/* Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<URSEL0)|(1<<UCSR0B)|(3<<UCSZ00);
	// Coupling printf to uart
	fdevopen(uart_transmit,uart_recieve);
	
}

int uart_recieve(FILE * file)
{
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	/* Get and return received data from buffer */
	return UDR0;
}

int uart_transmit(char data, FILE * file)
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
	return 0;
}


void main(void)
{
	DDRA = (1<<PA0);

    uart_init(MYUBBR);

	PORTA = (1<<PA0);

	printf("Ting funker");

}