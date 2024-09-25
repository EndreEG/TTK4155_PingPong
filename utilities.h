#pragma once
#define set_bit(reg , bit )( reg |= ( 1 << bit ))
#define clear_bit( reg , bit )( reg &= ~( 1 << bit ) )
#define test_bit( reg , bit )( reg & ( 1 << bit ) )
#define loop_until_bit_is_set( reg , bit ) while ( !test_bit ( reg , bit ) )
#define loop_until_bit_is_clear( reg , bit ) while ( test_bit ( reg , bit ) )
#include <stdio.h>
#include <avr/io.h>
#define F_CPU 4915200// Clock Speed


void XMEM_init();
void sram_test(void);
void ADC_init();
void ADC_get(uint8_t* ADC_values);