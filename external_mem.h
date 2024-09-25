#pragma once
#include "avr/io.h"
#define F_CPU 4915200//16000000
#include "util/delay.h"
#include "utilities.h"
#define ADC_START_ADDRESS 0x1400
#define SRAM_START_ADDRESS 0x1800


void sram_init();
void adc_init();
void test_led();
void sram_test(void);
void adc_test(void);
uint8_t adc_read_channel(uint8_t channel);

void write_sram(uint16_t address, uint8_t data);
uint8_t read_sram(uint16_t address);