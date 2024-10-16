#pragma once
#include "utilities.h"


// void test_led();
void sram_test(void);
uint8_t adc_read_channel(uint8_t channel);

void write_sram(uint16_t address, uint8_t data);
uint8_t read_sram(uint16_t address);