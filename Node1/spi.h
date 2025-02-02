#pragma once
#include "utilities.h"

void set_ss_low();
void set_ss_high();

void spi_init();
uint8_t spi_transceive(uint8_t data);
uint8_t spi_read();
void spi_write(uint8_t data);