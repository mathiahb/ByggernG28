#include "interrupts.h"

#include "MultifunctionBoard/adc.h"
#include "MultifunctionBoard/oled.h"
#include "ExternalInterface/gpio.h"
#include "CAN/CAN.h"

#include "stdio.h"
#include "stdint.h"

#include "avr/interrupt.h"

#include "../CAN_IDs.h"

ISR(INT0_vect, ISR_BLOCK){
    cli();

    interrupt_adc_end();
    
    Point point = get_analog_position();
    DIRECTION direction = get_digital_direction(point);

    CAN_Message message = {
        .ID = JOYSTICK_INFO, 
        .data_length = 5, 
        .data = {(uint8_t) direction, (uint8_t) point.x, (uint8_t) point.y, (uint8_t) point.left_slider, (uint8_t) point.right_slider}, 
        .remote_frame = 0
    };

    CAN_transmit(message);

    //GIFR = 0;

    return;
}

ISR(INT1_vect, ISR_BLOCK){
    cli();

    CAN_interrupt_handler();

    return;
}

ISR(TIMER3_COMPA_vect, ISR_BLOCK){
    cli();

    interrupt_adc_begin();    

    print_sram_to_oled();

    //TIMSK = 0;

    return;
}

ISR(BADISR_vect){
    for(;;) UDR0 = '!';
}

void init_interrupts(){
    if(MCUCSR & (1<<PORF )) printf("Power-on reset.\r\n");
    if(MCUCSR & (1<<EXTRF)) printf("External reset.\r\n");
    if(MCUCSR & (1<<BORF )) printf("Brownout reset.\r\n");
    if(MCUCSR & (1<<WDRF )) printf("Watchdog reset.\r\n");
    if(MCUCSR & (1<<JTRF )) printf("JTAG reset.\r\n");
    MCUCSR = 0;

    // Timer For ADC + OLED Interrupt
    // Set MODE of Timer 3 to 4
    TCCR3A |= (1 << COM3A0);

    TCCR3A &= ~(1 << WGM10) & ~(1 << WGM11);
    TCCR3B &= ~(1 << WGM13);
    TCCR3B |= (1 << WGM12);

    // Set Output Compare to X
    OCR3AH = 0x00;
    OCR3AL = 0x50;
    
    // set Clock Select to 6 (1024 times slower than CPU)
    TCCR3B &= ~(1 << CS31);
    TCCR3B |= (1 << CS30) | (1 << CS32);

    // Activate Interrupts
    MCUCR |= (3 << ISC00) | (2 << ISC10);
    GICR = (1 << INT0) | (1 << INT1);
    ETIMSK = (1 << OCIE3A);

    sei();
}
