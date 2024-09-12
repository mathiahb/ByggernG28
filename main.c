#include "gpio.h"
#include "uart.h"
#include "xmemory.h"

#include "stdio.h"

int main()
{
  init_uart();
  init_xmemory();

  volatile int sleep = 0;
  int latch_pin = 0;

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

    if((sleep % 5000) == 0){
        printf("Sleep: %d\n", sleep);
    }

    if (!sleep)
    {
      sleep = 10000;
      
      printf("Latch pin %d\n", latch_pin);

      XMEM[(1 << latch_pin)] = 0;
      latch_pin++;
      latch_pin %= 8;
    }

    --sleep;
  }

  return 0;
}