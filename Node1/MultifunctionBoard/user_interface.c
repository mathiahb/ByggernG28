#include "user_interface.h"
#include "menus.h"
#include "oled.h"
#include "adc.h"

#include "../ExternalInterface/gpio.h"
#include "../CAN/CAN.h"
#include "../../CAN_IDs.h"

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
    int current_clicked = !read_pin(B, 1);

    if(previous_clicked != 0 && current_clicked == 0){
        //(*cb_function)(current_menu, cursor_position);

        CAN_Message message = {.data = {}, .data_length = 0, .ID = SOLENOID_COMMAND_SHOOT, .remote_frame = 0};
        CAN_transmit(message);
    }

    previous_clicked = current_clicked;
}

void print_menu(const Menu* menu){
    for(int line = 0; line < 8; line++){
        oled_print((*menu).lines[line], 8, line);
    }
}

void print_cursor(){
    oled_print_arrow(3, 0, cursor_position);
}

void draw_menu(){
    print_menu(&start_menu);

    int previous_cursor = update_cursor();

    print_cursor();

    oled_print_arrow(0, 32, 5);
    oled_print_arrow(1, 40, 5);
    oled_print_arrow(2, 48, 5);
    oled_print_arrow(3, 56, 5);
}

void draw_smiley(){
    oled_circle(100, 44, 18);
    oled_line(100, 44 + 18, 100 - 15, 44 + 11);
    oled_line(100 - 15, 44 + 11, 100 - 15, 44 - 11);
    oled_line(100 - 15, 44 - 11, 100, 44 - 18);
    oled_line(100, 44 - 18, 100 + 15, 44 - 11);
    oled_line(100 + 15, 44 - 11, 100 + 15, 44 + 11);
    oled_line(100 + 15, 44 + 11, 100, 44 + 18);
}


void init_user_interface(){
    set_pin_as_input(B, 1); // Joystick button

    bind_draw_function(draw_menu);
    bind_draw_function(draw_smiley);
}

void update_user_interface(){
    check_clicked();
}