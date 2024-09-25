#pragma once
#include "avr/io.h"
#include "avr/pgmspace.h"
#include "stdlib.h"
#include "fonts.h"
#define F_CPU 4915200//16000000
#include "util/delay.h"

#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200
void oled_init();
void oled_print_arrow(uint8_t row, uint8_t col);
void oled_write_command(uint8_t command);
void oled_write_data(uint8_t data);
void oled_pos(uint8_t row, uint8_t col);
void extract_font(uint8_t *character_array, uint8_t character);
void oled_clear();
void oled_test();
void print_character(uint8_t character);
void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);

