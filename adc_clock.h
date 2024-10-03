#include "stdint.h"

typedef enum{
    NEUTRAL, UP, DOWN, LEFT, RIGHT
} DIRECTION;

typedef struct{
    int16_t x;
    int16_t y;
    int16_t left_slider;
    int16_t right_slider;
} Point;

void setup_adc_clock();

DIRECTION get_digital_direction(Point analog_direction);
Point get_analog_position();