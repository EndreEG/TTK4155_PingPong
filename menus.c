#include "menus.h"

void menu_init() {
    oled_clear();
    clear_bit(DDRB, DDB2); // Set PB2 as input for joystick button (redundant)
    set_bit(PORTB, PB2); // Enable pull-up resistor
}

void menu() {
    oled_clear();
    print_string("Menu", 0, 48, 0);
    print_string("", 1, 0, 0);
    print_string("1.Play", 2, ARROW_SPACE, 0);
    print_string("2.Highscore", 3, ARROW_SPACE, 0);
    print_string("3.Settings", 4, ARROW_SPACE, 0);
    print_string("4.Quit", 5, ARROW_SPACE, 0);
}

void invert_menu_item(uint8_t arrow_pos, uint8_t inverted) {
    switch (arrow_pos) {
        case 2:
            print_string("1.Play", 2, ARROW_SPACE, inverted);
            break;
        case 3:
            print_string("2.Highscore", 3, ARROW_SPACE, inverted);
            break;
        case 4:
            print_string("3.Settings", 4, ARROW_SPACE, inverted);
            break;
        case 5:
            print_string("4.Quit", 5, ARROW_SPACE, inverted);
            break;
        default:
            break;
    }
}

state main_menu(){
    menu();
	uint8_t current_arrow_pos = 1; // 1 is an invalid position for the arrow,
	uint8_t previous_arrow_pos = 2; // but is used to invert the first menu item
	joystick_direction direction = NEUTRAL;
	uint8_t *adc_readings = (uint8_t *) malloc(4);
	current_arrow_pos = move_arrow(current_arrow_pos, DOWN); // Move arrow down to initial position (first menu item/Play)
	printf("Entering main menu\n\r");
    while (1)
	{
		adc_read(adc_readings);
		direction = get_joystick_direction(adc_readings[0], adc_readings[1]);
		current_arrow_pos = move_arrow(current_arrow_pos, direction);
		if (current_arrow_pos != previous_arrow_pos) {
			previous_arrow_pos = current_arrow_pos;
            printf("Current arrow pos: %d\n\r", current_arrow_pos);
			_delay_ms(200);
		}
        if (get_joystick_button_pressed()) {
            printf("Button pressed\n\r");
            _delay_ms(200);
            free(adc_readings);
            adc_readings = NULL;
            return current_arrow_pos;
            break;
        }
	}
	// free(adc_readings);
	// adc_readings = NULL;
}

state play(){
    printf("Play\n\r");
    return MAIN_MENU;
}

state highscore(){
    printf("Highscore\n\r");
    return MAIN_MENU;
}

state settings(){
    printf("Settings\n\r");
    return MAIN_MENU;
}

state quit(){
    printf("Quit\n\r");
    return MAIN_MENU;
}

uint8_t get_joystick_button_pressed(){
    return !test_bit(PINB, PB2);
}

state state_machine(state current_state) {
    switch (current_state)
    {
    case MAIN_MENU:
        current_state = main_menu();
        break;
    case PLAY:
        current_state = play();
        break;
    case HIGHSCORE:
        current_state = highscore();
        break;
    case SETTINGS:
        current_state = settings();
        break;
    case EXIT:
        current_state = quit();
        break;
    default:
        current_state = MAIN_MENU;
        break;
    }
    return current_state;
}