#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

#include "ExternalInterface/gpio.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "UART/uart.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    //Uncomment after including uart above
    uart_init(84000000, 9600);
    //printf("Hello World\n\r");

    setup_pin_as_output(B, 13, 0);

    volatile uint32_t sleep = 1000;
    volatile uint32_t on = 1;

    while (1)
    {
        /* code */
        while(--sleep);

        sleep = 10000;

        write_output(B, 13, on);
        on = !on;

        uart_tx('A');
    }
    
}