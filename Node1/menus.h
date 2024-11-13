#pragma once
#include "oled.h"

void menu();
void menu_init();
state state_machine(state current_state);
state main_menu();
state play();
state highscore();
state settings();
state quit();