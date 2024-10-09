#pragma once
#include "utilities.h"

void spi_init();
uint8_t spi_transceive(uint8_t data);
uint8_t spi_read();
void spi_write(uint8_t data);