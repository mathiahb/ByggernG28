#include "stdint.h"

typedef void (*draw_function)();
void bind_draw_function(draw_function func);
void draw(); // Calls all bound draw functions. Called from main

void oled_init();
void oled_reset();
void oled_home();
void oled_clear_line(uint16_t line);
void oled_print_arrow(int direcition, uint16_t x, uint16_t y);
void oled_print(char* string, uint16_t x, uint16_t y);

// Drawing function :D
void oled_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
void oled_circle(int16_t x_center, int16_t y_center, int16_t radius);

void print_sram_to_oled(); // Called from interrupt routine to paint to OLED