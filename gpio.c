#include "gpio.h"

volatile int *PORTA = (int *)0x3B;
volatile int *PORTB = (int *)0x38;
volatile int *PORTC = (int *)0x35;
volatile int *PORTD = (int *)0x32;
volatile int *PORTE = (int *)0x27;

volatile int *DDRA = (int *)0x3A;
volatile int *DDRB = (int *)0x37;
volatile int *DDRC = (int *)0x34;
volatile int *DDRD = (int *)0x31;
volatile int *DDRE = (int *)0x26;

volatile int *PINA = (int *)0x39;
volatile int *PINB = (int *)0x36;
volatile int *PINC = (int *)0x33;
volatile int *PIND = (int *)0x30;
volatile int *PINE = (int *)0x25;

void set_pin_as_input(enum Port port, int pin)
{
  if (port == A)
  {
    *DDRA &= ~(1 << pin);
  }
  else if (port == B)
  {
    *DDRB &= ~(1 << pin);
  }
  else if (port == C)
  {
    *DDRC &= ~(1 << pin);
  }
  else if (port == D)
  {
    *DDRD &= ~(1 << pin);
  }
  else if (port == E)
  {
    *DDRE &= ~(1 << pin);
  }

  write_pin(port, pin, 1);
}
void set_pin_as_output(enum Port port, int pin)
{
  if (port == A)
  {
    *DDRA |= (1 << pin);
  }
  else if (port == B)
  {
    *DDRB |= (1 << pin);
  }
  else if (port == C)
  {
    *DDRC |= (1 << pin);
  }
  else if (port == D)
  {
    *DDRD |= (1 << pin);
  }
  else if (port == E)
  {
    *DDRE |= (1 << pin);
  }
}

void write_pin(enum Port port, int pin, int value)
{
  if (port == A)
  {
    if (value)
    {
  set_pin_as_output(D, 1);
  set_pin_as_input(D, 0);
      *PORTA |= (1 << pin);
    }
    else
    {
      *PORTA &= ~(1 << pin);
    }
  }
  else if (port == B)
  {
    if (value)
    {
      *PORTB |= (1 << pin);
    }
    else
    {
      *PORTB &= ~(1 << pin);
    }
  }
  else if (port == C)
  {
    if (value)
    {
      *PORTC |= (1 << pin);
    }
    else
    {
      *PORTC &= ~(1 << pin);
    }
  }
  else if (port == D)
  {
    if (value)
    {
      *PORTD |= (1 << pin);
    }
    else
    {
      *PORTD &= ~(1 << pin);
    }
  }
  else if (port == E)
  {
    if (value)
    {
      *PORTE |= (1 << pin);
    }
    else
    {
      *PORTE &= ~(1 << pin);
    }
  }
}

unsigned int read_pin(enum Port port, int pin)
{
  if (port == A)
  {
    return (*PINA & (1 << pin)) >> pin;
  }
  else if (port == B)
  {
    return (*PINB & (1 << pin)) >> pin;
  }
  else if (port == C)
  {
    return (*PINC & (1 << pin)) >> pin;
  }
  else if (port == D)
  {
    return (*PIND & (1 << pin)) >> pin;
  }
  else if (port == E)
  {
    return (*PINE & (1 << pin)) >> pin;
  }
}