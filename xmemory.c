#include "xmemory.h"
#include "gpio.h"

volatile char* XMEM = (char*) 0x1000;

volatile char* OLED = (char*) 0x2000;
volatile char* ADC = (char*) 0x1400;
volatile char* SRAM = (char*) 0x1800;

volatile unsigned char* EMCUCR = 0x56;

int SRW11 = 1;
int SRL0 = 4;
int SRL1 = 5;
int SRL2 = 6;
int SM0 = 7;

volatile unsigned char* MCUCR = (unsigned char*) 0x55;
int SRW10 = 6;
int SRE = 7;

volatile unsigned char* SFIOR = (unsigned char*) 0x50;

int XMM0 = 3;
int XMM1 = 4;
int XMM2 = 5;


void init_xmemory(){
    //for(int pin = 0; pin < 8; pin++){
    //    set_pin_as_output(A, pin);
    //    write_pin(A, pin, 0);
    //}

    *MCUCR |= (1 << SRE);
    *SFIOR &= ~(3 << XMM0);
    *SFIOR |= (1 << XMM2);
    //*EMCUCR &= ~(1 <<  SRL2) & ~(1 <<  SRL1) & ~(1 << SRW11);
    //*EMCUCR |= (1 << SRL0);
    //*MCUCR |= (1 << SRW10);

}