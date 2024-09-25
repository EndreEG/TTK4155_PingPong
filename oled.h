#pragma once
#include "fonts.h"
#include "utilities.h"

void oled_init();
void oled_print_arrow(uint8_t row, uint8_t col);
void oled_write_command(uint8_t command);
void oled_write_data(uint8_t data);
void oled_pos(uint8_t row, uint8_t col);
void extract_font(uint8_t *character_array, uint8_t character);
void oled_clear();
void oled_test();
void print_character(uint8_t character);
void print_string(char* string);
void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);

