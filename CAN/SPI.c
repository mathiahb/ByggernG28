#include "SPI.h"

#include "../gpio.h"

#include "stdint.h"

#include "avr/interrupt.h"

void init_spi(){
    set_pin_as_output(B, 4); // Slave Select
    set_pin_as_output(B, 5); // MOSI
    set_pin_as_input(B, 6); // MISO
    set_pin_as_output(B, 7); // SCK

    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void transmit_spi(uint8_t out){
    SPDR = out;

    while(!(SPSR & (1 << SPIF))); // Wait until transmission is complete.
}

uint8_t receive_spi(){
    const uint8_t dummy = (uint8_t) 0x0;

    transmit_spi(dummy);

    return SPDR;
}