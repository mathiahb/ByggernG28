#include "gpio.h"
#include "uart.h"

int main()
{
  /*volatile int *PORT_A = (int *)0x3B;
  volatile int *DDR_A = (int *)0x3A;
  volatile int *PIN_A = (int *)0x39;

  int PIN_0 = 0;

  *DDR_A = (1 << PIN_0);

  while (1)#define FOSC 1843200
#define BAUD 9600
#define MYUBBR FORSC/16/BAUD-1

  {
    *PORT_A |= (1 << PIN_0);

    volatile int sleep = 10000;

    while (sleep)
    {
      sleep--;
    }

    *PORT_A &= ~(1 << PIN_0);

    sleep = 10000;

    while (sleep)
    {
      sleep--;
    }
  }*/

  init_uart();

  set_pin_as_output(A, 0);
  set_pin_as_output(A, 1);
  set_pin_as_output(A, 2);
  int on = 1;

  volatile int sleep = 0;

  while (1)
  {
    if (uart_unread_data_in_buffer() && uart_ready_to_send())
    {
      char letter = receive_uart();
      send_uart(letter);
      write_pin(A, 2, on);
    }
    else if (uart_ready_to_send() && !sleep)
    {
      send_uart('a');
      write_pin(A, 1, on);
    }

    if (!sleep)
    {
      write_pin(A, 0, on);
      on = !on;
      sleep = 10000;
    }

    --sleep;
  }

  return 0;
}