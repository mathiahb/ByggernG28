#include "timercounter.h"

#include "../ExternalInterface/gpio.h"
#include "pmc.h"

#include "sam/sam3x/include/sam.h"

//const uint32_t RC = 13125;

void disable_write_protection(){
    REG_TC2_WPMR &= ~(TC_WPMR_WPEN);
}

void enable_write_protection(){
    REG_TC2_WPMR |= (TC_WPMR_WPEN);
}

void init_timercounter(uint32_t frequency_TC0){
    //init_PMC(); //Called by pwm module
    
   setup_pin_as_peripheral(C, 25, CONTROLLER_B);
   setup_pin_as_peripheral(C, 26, CONTROLLER_B);
   //setup_pin_as_peripheral(C, 27, CONTROLLER_B);

    disable_write_protection();

    // Interrupt timer
    REG_TC0_CMR0 = (TC_CMR_TCCLKS_TIMER_CLOCK1 |
                    TC_CMR_WAVE |
                    TC_CMR_WAVSEL_UP_RC
    );

    REG_TC0_CMR1 = (TC_CMR_TCCLKS_TIMER_CLOCK2 |
                    TC_CMR_WAVE |
                    TC_CMR_WAVSEL_UP |
                    TC_CMR_CPCSTOP);

    REG_TC0_RC0 = (84000000 / 2 / frequency_TC0); // Selected so that interrupt is generated every x ?seconds
    REG_TC0_RC1 = (328125);

    REG_TC0_IER0 = TC_IER_CPCS;
    REG_TC0_IER1 = TC_IER_CPCS;

    NVIC_EnableIRQ(ID_TC0);
    NVIC_EnableIRQ(ID_TC1);




    // Encoder Decoder
    REG_TC2_BMR = TC_BMR_QDEN | TC_BMR_POSEN;

    REG_TC2_CMR0 = (TC_CMR_TCCLKS_XC0 |
                    TC_CMR_ETRGEDG_RISING |
                    TC_CMR_ABETRG
    );

    //REG_TC2_CMR1 = (TC_CMR_TCCLKS_XC0 |
    //                TC_CMR_ETRGEDG_RISING |
    //                TC_CMR_ABETRG
    //);

    //REG_TC0_RC0 = RC;

    REG_TC2_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
    REG_TC2_CCR1 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    REG_TC2_BCR = TC_BCR_SYNC;

    REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;
    REG_TC0_CCR1 = TC_CCR_CLKEN;

    enable_write_protection();
}