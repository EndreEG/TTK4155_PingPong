#pragma once
#include "utilities.h"

void adc_init();
void adc_test();
void adc_read(uint8_t* adc_readings);
uint8_t map_slider_output(uint8_t slider_value);
int8_t calibration(); 