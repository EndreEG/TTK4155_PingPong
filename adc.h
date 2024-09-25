#pragma once
#include "utilities.h"

void adc_init();
void adc_test();
uint8_t read_adc(uint8_t channel, uint8_t address);
uint8_t adc_read_channel(uint8_t channel);
uint8_t map_slider_output(uint8_t slider_value);
int8_t calibration(); 