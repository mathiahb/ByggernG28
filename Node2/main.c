#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"

#include "ExternalInterface/gpio.h"
#include "PWM/servo.h"

#define F_CPU 84000000

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "UART/uart.h"
#include "CAN/can.h"  
#include "../CAN_IDs.h"

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(F_CPU, 9600);

    can_init((CanInit){.brp = F_CPU / BAUDRATE - 1, .phase1 = PHASE1, .phase2 = PHASE2, .propag = PROPAG, .sjw = SJW, .smp = 1}, 0); 

    init_PWM();

    printf("Hello World!\r\n");

    setup_pin_as_output(B, 13, 0);

    volatile uint32_t sleep = 10;
    volatile uint32_t on = 1;

    while (1)
    {
        /* code */
        CanMsg received_messsage = {};
        int new_message = 0;

        new_message = can_rx(&received_messsage);

        if(new_message){
            if(received_messsage.id == JOYSTICK_INFO){
                printf("%d\r\n", (int32_t)((int8_t) received_messsage.byte[1]));
                joystick_set_servo_position((int32_t)((int8_t) received_messsage.byte[1]));
            }
        }

        while(--sleep);

        sleep = 1000;

        write_output(B, 13, on);
        on = !on;

        //uart_tx('A');
    }
    
}