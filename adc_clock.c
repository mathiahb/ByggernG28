#include "adc_clock.h"
#include "gpio.h"
#include "xmemory.h"

#include "stdint.h"
#include "avr/interrupt.h"
#include "stdio.h"
#include "stdlib.h"

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

//With correction
//volatile int16_t x_adj = 0;
//volatile int16_t y_adj = 0;

// Calibration values
volatile uint8_t x_calibration = 0; 
volatile uint8_t y_calibration = 0;
volatile int16_t x_max_adjusted = 0;
volatile int16_t y_max_adjusted = 0;

Point get_analog_direction(){
    Point analog_position = (Point) {.x = 0, .y = 0, .left_slider = (int16_t) touch_a, .right_slider = (int16_t) touch_b};

    if (x > x_calibration){
        analog_position.x = (x-x_calibration)*100/(x_max_adjusted);
    }
    else{
        analog_position.x =(x*100)/x_calibration - 100;
    }

    if (y > y_calibration){
        analog_position.y =(y-y_calibration)*100/(y_max_adjusted);
    }
    else{
        analog_position.y =(y*100)/y_calibration - 100;
    }

    return analog_position;
}

DIRECTION get_digital_direction(Point analog_direction){
    const int16_t limit = 20;

    //Point analog_direction = get_analog_direction();

    if(abs(analog_direction.x) > abs(analog_direction.y)){
        if(analog_direction.x > limit){
            return RIGHT;
        }else if(analog_direction.x < -limit){
            return LEFT;
        }else{
            return NEUTRAL;
        }
    }else{
        if(analog_direction.y > limit){
            return UP;
        }else if(analog_direction.y < -limit){
            return DOWN;
        }else{
            return NEUTRAL;
        }
    }

    return -1;
}

ISR(INT0_vect, ISR_BLOCK){
    cli();

    y = ADC[0];
    x = ADC[0];
    touch_a = ADC[0];
    touch_b = ADC[0];

    Point analog_position = get_analog_direction();
    DIRECTION direction = get_digital_direction(analog_position);
    printf("x: %d, y: %d, a: %u, b: %u, d: %d \r\n", analog_position.x, analog_position.y, touch_a, touch_b, (int16_t) direction);

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
    if(MCUCSR & (1<<PORF )) printf("Power-on reset.\r\n");
    if(MCUCSR & (1<<EXTRF)) printf("External reset!\r\n");
    if(MCUCSR & (1<<BORF )) printf("Brownout reset!\r\n");
    if(MCUCSR & (1<<WDRF )) printf("Watchdog reset!\r\n");
    if(MCUCSR & (1<<JTRF )) printf("JTAG reset!\r\n");
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

    // Read correction
    ADC[0] = 0;
    while(!read_pin(D, 5)){
        volatile uint8_t sleep = 10;
        while(--sleep){}
    }
    y_calibration = ADC[0];
    x_calibration = ADC[0];

    x_max_adjusted = 255 - (int16_t) x_calibration;
    y_max_adjusted = 255 - (int16_t) y_calibration;

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