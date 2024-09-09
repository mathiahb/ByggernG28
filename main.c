#include "gpio.h"
#include "uart.h"

#include "stdio.h"

int main()
{
  init_uart();

  set_pin_as_output(A, 0);
  //set_pin_as_output(A, 1);
  //set_pin_as_output(A, 2);
  int on = 1;

  volatile int sleep = 0;

  while (1)
  {
    /*if (uart_unread_data_in_buffer() && uart_ready_to_send())
    {
      char letter = receive_uart();
      send_uart(letter);
      write_pin(A, 2, on);
    }
    else if (uart_ready_to_send() && !sleep)
    {
      send_uart('a');
      write_pin(A, 1, on);
    }*/

   printf("Sleep: %d\n", sleep);

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