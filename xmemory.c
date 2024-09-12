#include "xmemory.h"
#include "gpio.h"

volatile char* XMEM = (char*) 0x1000;

//volatile unsigned char* EMCUCR = 0x56;
volatile unsigned char* MCUCR = 0x55;

int SRE = 7;

volatile unsigned char* SFIOR = 0x50;

int XMM = 3;


void init_xmemory(){
    for(int pin = 0; pin < 8; pin++){
        set_pin_as_output(A, pin);
        write_pin(A, pin, 0);
    }

    *SFIOR |= (4 << XMM);

    *MCUCR |= (1 << SRE);
}