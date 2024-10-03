#include "stdint.h"

void oled_init();
void oled_reset();
void oled_home();
void oled_goto_line(uint16_t line);
void oled_goto_column(uint16_t column);
void oled_clear_line(uint16_t line);
void oled_pos(uint16_t row, uint16_t column);
void oled_print_arrow(int direcition);
void oled_print(char byte);