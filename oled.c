#include "oled.h"
#include "stdio.h"
#include "avr/interrupt.h"

#include "fonts.h"

volatile unsigned char* OLED_COMMAND_BASE = (unsigned char*) 0x2000;
volatile unsigned char* OLED_DATA = (unsigned char*) 0x2200;

uint16_t current_line = 0;
uint16_t current_column = 0;

void write_c(char command){
    *OLED_COMMAND_BASE = command;
}

void write_d(char data){
    *OLED_DATA = data;
    current_column += 1;
    if(current_column == 128) current_column = 0;
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
}

void oled_reset(){
    for(int i = 0; i<8; i++){
        oled_clear_line(i);
    }
    oled_pos(0, 0);
}

void oled_home(){

}

void oled_goto_line(uint16_t line){
    if(line < 8){
        uint8_t command = 0xB0 + line;
        write_c(command);
        current_line = line;
    }
    else{
        printf("!");
    }
    
}

void oled_goto_column(uint16_t column){
    if(column < 128){
        uint8_t lower_bits = (0x0F & column);
        uint8_t higher_bits = (0xF0 & column)>>4;
        uint8_t lower_command = (0x00 + lower_bits);
        uint8_t higher_command = (0x10 + higher_bits);
        write_c(lower_command);
        write_c(higher_command);

        current_column = column;
    }
    else{
        printf("!");
    }
}

void oled_clear_line(uint16_t line){
    oled_goto_line(line);
    for(int i = 0; i<128; i++){
        write_d(0x00);
    }

}
void oled_pos(uint16_t row, uint16_t column){
    oled_goto_line(row);
    oled_goto_column(column);
}

void oled_print_arrow(int direction){
    if(direction < 4){
        for(int i = 0; i < 8; i++){
            write_d(pgm_read_byte(&(arrows[direction][i])));
        }
    }
}

void oled_print(char letter){
    if(letter == '\r'){
        oled_goto_column(0);
        return;
    }
    if(letter == '\n'){
        if(current_line == 7){
            oled_goto_line(0);
        }else{
            oled_goto_line(current_line + 1);
        }
    }

    if(letter == '\t'){
        // Letter size * tabulator_distance
        const int tab_modulo = 8 * 4;
        for(int i = current_column % tab_modulo; i < 32; i++){
            write_d(0);
        }
    }
    

    uint8_t index = letter - 0x20;

    if(index > 94) return; // Invalid character

    for(int i = 0; i < 8; i++){
        write_d(pgm_read_byte(&(font8[index][i])));
    }
}