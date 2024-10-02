#include "oled.h"

void oled_init()
{
    oled_write_command(0xae); // display off
    oled_write_command(0xa1); //segment remap
    oled_write_command(0xda); //common pads hardware: alternative
    oled_write_command(0x12);
    oled_write_command(0xc8); //common output scan direction:com63~com0
    oled_write_command(0xa8); //multiplex ration mode:63
    oled_write_command(0x3f);
    oled_write_command(0xd5); //display divide ratio/osc. freq. mode
    oled_write_command(0x80);
    oled_write_command(0x81); //contrast control
    oled_write_command(0x50);
    oled_write_command(0xd9); //set pre-charge period
    oled_write_command(0x21);
    oled_write_command(0x20); //Set Memory Addressing Mode
    oled_write_command(0x02); // Page addressing mode
    oled_write_command(0xdb); //VCOM deselect level mode
    oled_write_command(0x30);
    oled_write_command(0xad); //master configuration
    oled_write_command(0x00);
    oled_write_command(0xa4); //out follows RAM content
    oled_write_command(0xa6); //set normal display
    oled_write_command(0xaf); // display on
}

void oled_clear_line(uint8_t page) {
    for (int col = 0; col < 128; col++) {
        oled_pos(page, col);
        oled_write_data(0b00000000);
    }
}

void oled_clear() {
    for (int page = 0; page < 8; page++) {
        for (int col = 0; col < 128; col++) {
            oled_pos(page, col);
            oled_write_data(0b00000000);
            // oled_write_data(0b11111111);
        }
    }    
}

void print_character(uint8_t character) {
    uint8_t* character_array = (uint8_t*) malloc(8); 
    extract_font(character_array, character);
    for (int i = 0; i < 8; i++) {
        oled_write_data(character_array[i]);
    }
    free(character_array);
    character_array = NULL;
}

void print_string(char* string, uint8_t start_page, uint8_t start_col) {
    int i = 0;
    while (string[i] != '\0') {
        oled_pos(start_page, start_col + 8*i);
        print_character(string[i]);
        i++;
    }
}



void extract_font(uint8_t *character_array, uint8_t character) {
    // uint8_t character[8];
    for (int i = 0; i < 8; i++) {
        character_array[i] = pgm_read_byte(&(font8[get_font8_index(character)][i]));
    }
}

void oled_test() {
    for (int i = 0; i < 128; i+=5) {
        oled_print_arrow(0, i);
        _delay_ms(300);
        }
}

void oled_print_arrow ( uint8_t page , uint8_t col )
{
    oled_pos ( page , col ) ;
    oled_write_data (0b00011000 ) ;
    oled_write_data (0b00011000 ) ;
    oled_write_data (0b01111110 ) ;
    oled_write_data (0b00111100 ) ;
    oled_write_data (0b00011000 ) ;
}

void oled_write_command(uint8_t command)
{
    volatile char *oled = (char *) OLED_COMMAND_ADDRESS;
    oled[0] = command;
}


void oled_write_data(uint8_t data)
{
    volatile char *oled = (char *) OLED_DATA_ADDRESS;
    oled[0] = data;
}

void oled_goto_page(uint8_t page) {
	if (0 <= page && page <= 7) {
		oled_write_command(0xB0 + page);
	}
}

void oled_goto_column(uint8_t column) {
    if (0 <= column && column <= 127) {
        oled_write_command(0x00 + column % 16);
        oled_write_command(0x10 + column / 16);
    }
}

void oled_pos(uint8_t page, uint8_t col) {
    //printf("Page: %d, Col: %d\n\r", page, col);
    oled_goto_page(page);
    oled_goto_column(col);
}

void menu() {
    oled_clear();
    print_string("Menu", 0, 48);
    print_string("", 1, 0);
    print_string("1.Play", 2, ARROW_SPACE);
    print_string("2.Highscore", 3, ARROW_SPACE);
    print_string("3.Settings", 4, ARROW_SPACE);
    print_string("4.Exit", 5, ARROW_SPACE);
}

state main_menu(){
    menu();
	uint8_t current_arrow_pos = 2;
	uint8_t previous_arrow_pos = 2;
	joystick_direction direction = NEUTRAL;
	uint8_t *adc_readings = (uint8_t *) malloc(4);
	oled_print_arrow(current_arrow_pos, 0);
	while (1)
	{
		adc_read(adc_readings);
		direction = get_joystick_direction(adc_readings[0], adc_readings[1]);
		current_arrow_pos = move_arrow(current_arrow_pos, direction);
		printf("Current arrow pos: %d Previous arrow pos: %d Direction: %d\n\r", current_arrow_pos, previous_arrow_pos, direction);
		if (current_arrow_pos != previous_arrow_pos) {
			previous_arrow_pos = current_arrow_pos;
			_delay_ms(200);
		}
        if (get_joystick_button_pressed()) {
            return current_arrow_pos;
            break;
        }
	}
	free(adc_readings);
	adc_readings = NULL;
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
    return !test_bit(PINB, PB7);
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

uint8_t move_arrow(uint8_t arrow_pos, int8_t direction) {
    if (direction == NEUTRAL || direction == RIGHT || direction == LEFT) {
        return arrow_pos;
    }
    clear_arrow(arrow_pos);
    if (direction == UP) {
        if(arrow_pos - 1 < START_POS_MENU){
            arrow_pos = END_POS_MENU;
        }
        else arrow_pos--;
    }
    else if (direction == DOWN) {
        if(arrow_pos + 1 > END_POS_MENU){
            arrow_pos = START_POS_MENU;
        }
        else arrow_pos++;
    }
    oled_print_arrow(arrow_pos, 0);
    printf("Arrow pos: %d, Direction: %d\n\r", arrow_pos, direction);
    return arrow_pos;
}

void clear_arrow(uint8_t arrow_pos) {
    for (int col = 0; col < 8; col++)
    {
        oled_pos(arrow_pos, col);
        oled_write_data(0b00000000);
    }
}