#include "oled.h"
#include "stdio.h"
#include "avr/interrupt.h"

#include "fonts.h"

volatile unsigned char* OLED_COMMAND_BASE = (unsigned char*) 0x2000;
volatile unsigned char* OLED_DATA = (unsigned char*) 0x2200;

volatile unsigned char* OLED_SRAM_DATA = 0x1C00;
volatile unsigned char* OLED_SRAM_CURRENT_DISPLAY = 0x1800;

volatile uint16_t drawing = 0;


draw_function draw_functions[16] = {};
uint16_t num_of_draw_functions = 0;

void bind_draw_function(draw_function func){
    draw_functions[num_of_draw_functions] = func;
    num_of_draw_functions++;
}

void draw() // update OLED
{
    for(int i = 0; i < num_of_draw_functions; i++){
        draw_functions[i]();
    }

    swap_sram_copies();
}

void write_c(char command){
    *OLED_COMMAND_BASE = command;
}

void write_d(char data){
    *OLED_DATA = data;
}

void write_data_to_sram_copy(uint8_t data, uint16_t x, uint16_t y){
    OLED_SRAM_DATA[y * 128 + x] = data;
}

void swap_sram_copies(){
    char* temp = OLED_SRAM_DATA;
    OLED_SRAM_DATA = OLED_SRAM_CURRENT_DISPLAY;
    OLED_SRAM_CURRENT_DISPLAY = temp; 

    oled_reset();
}

void oled_init(){
    write_c(0xae); // display off
    write_c(0xa1); //segment remapstring
    write_c(0xda); //common pads hardware: alternative
    write_c(0x12);
    write_c(0xc8); //common output scan direction:com63~com0
    write_c(0xa8); //multiplex ration mode:63
    write_c(0x3f);
    write_c(0xd5); //display divide ratio/osc. freq. mode
    write_c(0x80);
    write_c(0x81); //contrast control
    write_c(0x50);
    write_c(0xd9); //set pre-charge period
    write_c(0x21);
    write_c(0x20); //Set Memory Addressing Mode
    write_c(0x02);
    write_c(0xdb); //VCOM deselect level mode
    write_c(0x30);
    write_c(0xad); //master configuration
    write_c(0x00);
    write_c(0xa4); //out follows RAM content
    write_c(0xa6); //set normal display
    write_c(0xaf); // display on

    oled_reset();
}

void oled_reset(){
    for(int i = 0; i<8; i++){
        oled_clear_line(i);
    }
}

void oled_home(){

}

void oled_clear_line(uint16_t line){
    for(int i = 0; i<128; i++){
        write_data_to_sram_copy(0x00, i, line);
    }
}

void oled_print_arrow(int direction, uint16_t x, uint16_t y){
    if(direction < 4){
        for(int i = 0; i < 8; i++){
            write_data_to_sram_copy(pgm_read_byte(&(arrows[direction][i])), x, y);
            x++;
        }
    }
}

void oled_print(char* string, uint16_t x, uint16_t y){
    for(int letter = 0; string[letter]; letter++)
    {

        if(string[letter] == '\r'){
            x = 0;
            continue;
        }
        if(string[letter] == '\n'){
            if(y == 7){
                y = 0;
            }else{
                y += 1;
            }
            continue;
        }

        if(string[letter] == '\t'){
            // Letter size * tabulator_distance
            const int tab_modulo = 8 * 4;
            for(int i = x % tab_modulo; i < 32; i++){
                write_data_to_sram_copy(0, x, y);
                x++;
            }
            continue;
        }
        

        uint8_t index = string[letter] - 0x20;

        if(index > 94) continue; // Invalid character

        for(int i = 0; i < 8; i++){
            write_data_to_sram_copy(pgm_read_byte(&(font8[index][i])), x, y);
            x++;
        }

        if(letter >= 32){ 
            printf("SRAM overload in print_oled!\r\n");
            return;
        }
    }
}

void set_pixel_at(uint16_t x, uint16_t y){
    uint8_t bit = 1 << (y % 8);
    uint8_t row = y / 8;
    uint16_t index = row * 128 + x;
    OLED_SRAM_DATA[index] |= bit;
}

// Line Drawing Algorithm
// From Wikipedia, link can be provided
void oled_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2){
    int16_t dx = x2 - x1;
    int16_t dy = y2 - y1;

    if(dx == 0 && dy == 0){
        set_pixel_at(x1, y1);
        return;
    }

    if(dx != 0){
        if(dx < 0){
            int16_t temp = x1;
            x1 = x2;
            x2 = temp;

            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        for(int16_t x = x1; x <= x2; x++){
            int16_t y = (x - x1) * dy / dx + y1;
            set_pixel_at(x, y);
        }
    }else{
        if(dy < 0){
            int16_t temp = x1;
            x1 = x2;
            x2 = temp;

            temp = y1;
            y1 = y2;
            y2 = temp;
        }

        for(int16_t y = y1; y <= y2; y++){
            int16_t x = (y - y1) * dx / dy + x1;
            set_pixel_at(x, y);
        }
    }
}

void draw_all_8_symmetric_pixels_in_circle(uint16_t x, uint16_t y, uint16_t x_center, uint16_t y_center){
    uint16_t x_left = x_center - x;
    uint16_t x_right = x_center + x;
    uint16_t y_up = y_center - y; 
    uint16_t y_down = y_center + y;

    uint16_t x_up = x_center - y;
    uint16_t x_down = x_center + y;
    uint16_t y_left = y_center - x;
    uint16_t y_right = y_center + x;

    //if(x_left < 0) x_left = x_right;
    //if(x_right >= 128) x_right = x_left;
    //if(y_up < 0) y_up = y_down;
    //if(y_down >= 64) y_down = y_up;

    set_pixel_at(x_right, y_down);
    set_pixel_at(x_left, y_down);

    set_pixel_at(x_right, y_up);
    set_pixel_at(x_left, y_up);

    set_pixel_at(x_down, y_right);
    set_pixel_at(x_up, y_right);

    set_pixel_at(x_down, y_left);
    set_pixel_at(x_up, y_left);
}

// Midpoint Circle Algorithm
// Adapted from GeekforGeeks, link can be provided
void oled_circle(int16_t x_center, int16_t y_center, int16_t radius){
    int16_t x = radius, y = 0;

    int16_t perimeter = 1 - radius;

    draw_all_8_symmetric_pixels_in_circle(x, y, x_center, y_center);

    while(x > y){
        y++;

        if(perimeter <= 0){
            perimeter = perimeter + 2 * y + 1;
        }else{
            x--;
            perimeter = perimeter + 2*y - 2*x + 1;
        }

        if(x < y) break;

        draw_all_8_symmetric_pixels_in_circle(x, y, x_center, y_center);
    }
}


void print_sram_to_oled(){
    uint8_t lower_command = (0x00);
    uint8_t higher_command = (0x10);
    write_c(lower_command);
    write_c(higher_command);


    for(int line = 0; line < 8; line++){
        uint8_t command = 0xB0 + line;
        write_c(command);

        for(int column = 0; column < 128; column++){
            write_d(OLED_SRAM_CURRENT_DISPLAY[line * 128 + column]);
        }
    }
}