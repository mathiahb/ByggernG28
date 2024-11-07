#include "ExternalInterface/gpio.h"

void init_solenoid(){
    setup_pin_as_output(C, 28, 1);
}

void shoot_solenoid(){
    

    write_output(C, 28, 0);

    volatile sleep = 1000000;
    while(--sleep);

    write_output(C, 28, 1);
}