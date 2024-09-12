#include "uart.h"
#include "gpio.h"

#include "stdio.h"

// Hentet fra databladet
#define FOSC 4915200
#define BAUD 9600
#define UBRR (FOSC / 16 / BAUD - 1)

// Register B
int TXEN = 3;
int RXEN = 4;

// Register A
int UDRE = 5;
int TXC = 6;
int RXC = 7;

// Register C
int UCSZ0 = 1;
int USBS = 3;
int URSEL = 7;

volatile unsigned char *UCSR0A = (unsigned char *)0x2B;
volatile unsigned char *UCSR0B = (unsigned char *)0x2A;
volatile unsigned char *UCSR0C = (unsigned char *)0x40;

volatile unsigned char *UBRR0H = (unsigned char *)0x40;
volatile unsigned char *UBRR0L = (unsigned char *)0x29;

volatile unsigned char *UDR0 = (unsigned char *)0x2C;

void init_uart()
{
  set_pin_as_output(D, 1);
  set_pin_as_input(D, 0);

  // Trenger UBBRH å inneholde største verdier
  // UBBRL å inneholde minste verdier
  // Baudrate

  // Hentet fra databladet
  *UBRR0H = (unsigned char) ((UBRR) >> 8);
  *UBRR0L = (unsigned char) (UBRR);

  *UCSR0B = (1 << RXEN) | (1 << TXEN);

  *UCSR0C = (1 << URSEL) | (1 << USBS) | (3 << UCSZ0);

  fdevopen(guarantee_send_uart, receive_uart);
}

void send_uart(char letter)
{
  if (!uart_ready_to_send())
  {
    return;
  }

  *UDR0 = letter;
}

void guarantee_send_uart(char letter)
{
  while (!uart_ready_to_send()){}

  *UDR0 = letter;
}


char receive_uart()
{
  if (!uart_unread_data_in_buffer())
  {
    return 0;
  }

  return *UDR0;
}

int uart_ready_to_send() { return (*UCSR0A) & (1 << UDRE); }

int uart_unread_data_in_buffer() { return (*UCSR0A) & (1 << RXC); }