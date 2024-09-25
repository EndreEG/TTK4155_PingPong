/*
 * main.c
 *
 * Created: 9/4/2024 3:15:02 PM
 *  Author: endreeg
 */ 

#pragma once
#include "utilities.h"
//#include <xc.h>


//#define F_CPU 16000000
//#include "util/delay.h"

void uart_init( unsigned int ubrr );
int uart_recieve(FILE * file);
int uart_transmit(char data, FILE * file);