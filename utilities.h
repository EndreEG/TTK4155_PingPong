#pragma once
#include "stdlib.h"
#include "stdio.h"
#include "avr/io.h"
#include <avr/pgmspace.h>

#define F_CPU 4915200//16000000
#include "util/delay.h"

#define FOSC 4915200 // Crystal oscillator frequency
#define BAUDRATE 9600
#define MYUBBR FOSC/16/BAUDRATE - 1

#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200
#define ADC_START_ADDRESS 0x1400
#define SRAM_START_ADDRESS 0x1800

#define set_bit(reg , bit )( reg |= ( 1 << bit ))
#define clear_bit( reg , bit )( reg &= ~( 1 << bit ) )
#define test_bit( reg , bit )( reg & ( 1 << bit ) )
#define loop_until_bit_is_set( reg , bit ) while ( !test_bit ( reg , bit ) )
#define loop_until_bit_is_clear( reg , bit ) while ( test_bit ( reg , bit ) )

void xmem_init();
int get_font8_index(char c);
