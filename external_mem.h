#pragma once
#include "avr/io.h"
#define F_CPU 16000000
#include "util/delay.h"
#define OLED_START_ADDRESS 0x1000
#define ADC_START_ADDRESS 0x1400
#define SRAM_START_ADDRESS 0x1800

void sram_init();
void test_led();
void SRAM_test(void);

void write_sram(uint16_t address, uint8_t data);
uint8_t read_sram(uint16_t address);