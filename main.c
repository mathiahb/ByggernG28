#include "gpio.h"
#include "uart.h"
#include "xmemory.h"
#include "adc_clock.h"

#include "stdio.h"
#include "stdint.h"
#include <stdlib.h>

#ifdef SRAM_TEST
// From Blackboard.

void SRAM_test(void){
  volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
      uint16_t ext_ram_size = 0x800;
      uint16_t write_errors = 0;
      uint16_t retrieval_errors = 0;
      printf("Starting SRAM test...\r\n");
      // rand() stores some internal state, so calling this function in a loop will
      // yield different seeds each time (unless srand() is called before this function)
      uint16_t seed = rand();
      // Write phase: Immediately check that the correct value was stored
      srand(seed);
      for (uint16_t i = 0; i < ext_ram_size; i++) {
          uint8_t some_value = rand();
          ext_ram[i] = some_value;
          uint8_t retreived_value = ext_ram[i];
          if (retreived_value != some_value) {
              printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
              write_errors++;
          }
      }
      // Retrieval phase: Check that no values were changed during or after the write phase
      srand(seed);
      // reset the PRNG to the state it had before the write phase
      for (uint16_t i = 0; i < ext_ram_size; i++) {
          uint8_t some_value = rand();
          uint8_t retreived_value = ext_ram[i];
          if (retreived_value != some_value) {
              printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\r\n", i, retreived_value, some_value);
              retrieval_errors++;
          }
      }
      printf("SRAM test completed with \r\n%4d errors in write phase and \r\n%4d errors in retrieval phase\r\n\r\n", write_errors, retrieval_errors);
}
#endif

#ifdef DECODER_TEST
volatile char* OLED = (char*) 0x1000;
volatile char* ADC = (char*) 0x1400;
volatile char* SRAM = (char*) 0x1800;

int two_on = 0;
int three_on = 0;
#endif

int main()
{
  init_uart();

  setup_adc_clock();

#ifndef DECODER_TEST
  init_xmemory();
#endif

  volatile int sleep = 0;
  //uint16_t mem_sel = 0;

  while (1)
  {
#ifdef DECODER_TEST
    set_pin_as_output(C, 2);
    set_pin_as_output(C, 3);

    if(uart_unread_data_in_buffer()){
      char letter = receive_uart();
      if(letter == '2'){
        two_on = !two_on;
        write_pin(C, 2, two_on);
      }

      if(letter == '3'){
        three_on = !three_on;
        write_pin(C, 3, three_on);
      }
    }
#endif

    if((sleep % 5000) == 0){
        //printf("Sleep: %d\r\n", sleep);
    }

    if (!sleep)
    {
      sleep = 100000;

#ifdef SRAM_TEST
      SRAM_test();
#endif
    }
    --sleep;
  }

  return 0;
}