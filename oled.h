#pragma once
#include "joystick.h"
#include "fonts.h"
#include "utilities.h"

#define ARROW_SPACE 8
#define START_POS_MENU 2
#define END_POS_MENU 5

typedef enum {
    MAIN_MENU=1,
    PLAY,
    HIGHSCORE,
    SETTINGS,
    EXIT
} state;



void oled_init();
void oled_print_arrow(uint8_t row, uint8_t col);
void oled_write_command(uint8_t command);
void oled_write_data(uint8_t data);
void oled_pos(uint8_t row, uint8_t col);
void extract_font(uint8_t *character_array, uint8_t character);
void oled_clear();
void oled_clear_line(uint8_t page);
void oled_test();
void print_character(uint8_t character);
void print_string(char* string, uint8_t page, uint8_t col);
void oled_goto_page(uint8_t page);
void oled_goto_column(uint8_t column);
void menu();
uint8_t get_joystick_button_pressed();
state state_machine(state current_state);
state main_menu();
state play();
state highscore();
state settings();
state quit();
void clear_arrow(uint8_t arrow_pos);
uint8_t move_arrow(uint8_t arrow_pos, int8_t direction);

