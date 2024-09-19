#include "adc_clock.h"
#include "gpio.h"
#include "xmemory.h"

#include "stdint.h"
#include "avr/interrupt.h"
#include "stdio.h"

// ---------------- ADC CLOCK REGISTERS ----------------
//volatile unsigned char* TCCR1A = (unsigned char*) 0x4F;
//uint8_t COM1A1 = 7;
//uint8_t WGM10 = 0;
//uint8_t WGM11 = 1;

//volatile unsigned char* TCCR1B = (unsigned char*) 0x4E;
//uint8_t CS10 = 0;
//uint8_t CS11 = 1;
//uint8_t CS12 = 2;
//uint8_t WGM12 = 3;
//uint8_t WGM13 = 4;

//volatile unsigned char* OCR1AH = (unsigned char*) 0x4B;
//volatile unsigned char* OCR1AL = (unsigned char*) 0x4A;

//volatile unsigned char* ICR1H = (unsigned char*) 0x45;
//volatile unsigned char* ICR1L = (unsigned char*) 0x44;

// -----------------------------------------------------
// ----------- ADC INTERRUPT CLOCK REGISTERS------------

//volatile unsigned char* TCCR3A = (unsigned char*) 0x8B;
//volatile unsigned char* TCCR3B = (unsigned char*) 0x8A;

//volatile unsigned char* OCR3AH = (unsigned char*) 0x87;
//volatile unsigned char* OCR3AL = (unsigned char*) 0x86;

//volatile unsigned char* ICR3H = (unsigned char*) 0x81;
//volatile unsigned char* ICR3L = (unsigned char*) 0x80;

// -----------------------------------------------------
// ---------- ADC INTERRUPT VECTOR REGISTERS -----------

//volatile unsigned char* MCUCR = (unsigned char*) 0x55;
//int ISC0 = 0;
//int ISC1 = 2;
//volatile unsigned char* EMCUCR = (unsigned char*) 0x56;
// int ISC2 = 0; // SE DOKUMENTASJON, DEN OPPFØRER SEG ANNERLEDES.

//volatile unsigned char* GICR = (unsigned char*) 0x5B;

//int INTC0 = 6;
//int INTC1 = 7;
//int INTC2 = 5;

//volatile unsigned char* GIFR = (unsigned char*) 0x5A;

//int INTF0 = 6;
//int INTF1 = 7;
//int INTF2 = 5;

volatile uint8_t x = 0;
volatile uint8_t y = 0;
volatile uint8_t touch_a = 0;
volatile uint8_t touch_b = 0;

ISR(INT0_vect, ISR_BLOCK){
    cli();

    y = ADC[0];
    x = ADC[0];
    touch_a = ADC[0];
    touch_b = ADC[0];

    printf("x: %u, y: %u, a: %u, b: %u \r\n", x, y, touch_a, touch_b);

    GIFR = 0;

    return;
}

ISR(TIMER3_COMPA_vect, ISR_BLOCK){
    cli();

    ADC[0] = 0;

    TIMSK = 0;

    return;
}

ISR(BADISR_vect){
    for(;;) UDR0 = '!';
}


void setup_adc_clock(){
    if(MCUCSR & (1<<PORF )) printf("Power-on reset.\n");
    if(MCUCSR & (1<<EXTRF)) printf("External reset!\n");
    if(MCUCSR & (1<<BORF )) printf("Brownout reset!\n");
    if(MCUCSR & (1<<WDRF )) printf("Watchdog reset!\n");
    if(MCUCSR & (1<<JTRF )) printf("JTAG reset!\n");
    MCUCSR = 0;

    set_pin_as_input(D, 5);

    // Set MODE of Timer 1 to 14 (Fast PWM with ICR as TOP) 
    TCCR1A &= ~(1 << WGM10);
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);

    TCCR1B |= (1 << WGM12) | (1 << WGM13);

    // Set Output Compare to 1
    OCR1AH = 0;
    OCR1AL = 1;

    // Set TOP to 3
    ICR1H = 0;
    ICR1L = 3;

    // Set Clock Select to 1 (CLK_t = CLK_io)
    TCCR1B &= ~(1 << CS10) & ~(1 << CS12);
    TCCR1B |= (1 << CS11);

    // Activate signal.
    set_pin_as_output(D, 5);

    // Set MODE of Timer 3 to 4
    TCCR3A |= (1 << COM3A0);

    TCCR3A &= ~(1 << WGM10) & ~(1 << WGM11);
    TCCR3B &= ~(1 << WGM13);
    TCCR3B |= (1 << WGM12);

    // Set Output Compare to X
    OCR3AH = 0x4;
    OCR3AL = 0x50;
    
    // set Clock Select to 6 (16 times slower than CPU)
    TCCR3B &= ~(1 << CS11);
    TCCR3B |= (1 << CS10) | (1 << CS12);


    MCUCR |= (3 << ISC00);
    GICR = (1 << INT0);
    ETIMSK = (1 << OCIE3A);

    sei();
}