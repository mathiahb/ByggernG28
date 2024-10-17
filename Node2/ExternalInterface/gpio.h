#include "stdint.h"

typedef enum{
    A, B, C, D, E, F
} PORT;

void setup_pin_as_output(PORT port, uint32_t pin, uint32_t starting_value);
void write_output(PORT port, uint32_t pin, uint32_t value);