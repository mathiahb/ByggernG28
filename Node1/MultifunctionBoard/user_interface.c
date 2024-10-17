#include "user_interface.h"
#include "menus.h"
#include "oled.h"
#include "adc.h"

#include "../ExternalInterface/gpio.h"

#include "stdio.h"

DIRECTION previous_direction = NEUTRAL;
int16_t cursor_position = 0;
const int16_t max_cursor_position = 7;
MENU_ID current_menu = START_MENU;

int previous_clicked = 1;


void (*cb_function)(MENU_ID menu_id, int line) = 0;

void bind_callback(void (*callback_function)(MENU_ID, int)){
    cb_function = callback_function;
}

int update_cursor(){
    int previous_cursor = cursor_position;

    DIRECTION current_direction = get_digital_direction(get_analog_position());

    if(previous_direction != current_direction){
        if(current_direction == UP){
            if(cursor_position) cursor_position -= 1;
            else cursor_position = max_cursor_position;
        }
        if(current_direction == DOWN){
            if(cursor_position == max_cursor_position) cursor_position = 0;
            else cursor_position += 1;
        }
    }

    previous_direction = current_direction;

    return previous_cursor;
}

void check_clicked(){
    int current_clicked = read_pin(B, 3);

    if(previous_clicked != 0 && current_clicked == 0){
        (*cb_function)(current_menu, cursor_position);
    }

    previous_clicked = current_clicked;
}

void print_menu(const Menu menu){
    for(int line = 0; line < 8; line++){
        oled_pos(line, 8);
        for(int character_location_in_line = 0; character_location_in_line < 15; character_location_in_line++){
            oled_print(menu.lines[line][character_location_in_line]);
        }
    }
}

void print_cursor(int previous_cursor){
    if(cursor_position == previous_cursor){
        return;
    }
    oled_pos(previous_cursor, 0);
    oled_print(' ');

    oled_pos(cursor_position, 0);
    oled_print_arrow(3);
}

void init_user_interface(){
    set_pin_as_input(B, 3); // Joystick button
    print_menu(start_menu);
    print_cursor(-1);
}

void update(){
    int previous_cursor = update_cursor();

    check_clicked();

    print_cursor(previous_cursor);

    oled_pos(4, 64);
    oled_print_arrow(0);
    oled_print_arrow(1);
    oled_print_arrow(2);
    oled_print_arrow(3);
}