enum Port
{
  A = 0,
  B = 1,
  C = 2,
  D = 3,
  E = 4
};

void set_pin_as_input(enum Port port, int pin);
void set_pin_as_output(enum Port port, int pin);

void write_pin(enum Port port, int pin, int value);
unsigned int read_pin(enum Port port, int pin);