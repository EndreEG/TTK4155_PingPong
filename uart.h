/*
 * main.c
 *
 * Created: 9/4/2024 3:15:02 PM
 *  Author: endreeg
 */ 

#include <stdio.h>
//#include <xc.h>
#include "avr/io.h"

#define FOSC 4915200 // Crystal oscillator frequency
#define BAUDRATE 9600
#define MYUBBR FOSC/16/BAUDRATE - 1
//#define F_CPU 16000000
//#include "util/delay.h"

void uart_init( unsigned int ubrr );
int uart_recieve(FILE * file);
int uart_transmit(char data, FILE * file);