#pragma once
#include "oled.h"

void menu();
uint8_t get_joystick_button_pressed();
state state_machine(state current_state);
state main_menu();
state play();
state highscore();
state settings();
state quit();