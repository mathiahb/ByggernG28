#include "timercounter.h"

#include "../ExternalInterface/gpio.h"
#include "pmc.h"

#include "sam/sam3x/include/sam.h"

const uint32_t RC = 1000;
const uint32_t scalar = 1; // Scaling RA, RB and RC equally.

void disable_write_protection(){
    REG_TC0_WPMR &= ~(TC_WPMR_WPEN);
}

void enable_write_protection(){
    REG_TC0_WPMR |= (TC_WPMR_WPEN);
}

// Takes value between 0 and 1000. 
// promille = D*1000
void set_duty_cycle_A(uint32_t promille){
    disable_write_protection();

    REG_TC0_RA0 = promille * scalar;

    enable_write_protection();
}

void init_timercounter(){
    init_PMC();
    
    setup_pin_as_peripheral(B, 25, CONTROLLER_B);

    disable_write_protection();

    // Setup TIOA so that RA = D * RC
    // TIOB Deactivated
    REG_TC0_CMR0 = ((TC_CMR_TCCLKS_TIMER_CLOCK1 << TC_CMR_TCCLKS_Pos) // Clock select
                    //| TC_CMR_CLKI // Clock Invert
                    | TC_CMR_BURST_NONE
                    //| TC_CMR_CPCSTOP
                    //| TC_CMR_CPCDIS
                    | TC_CMR_EEVTEDG_NONE // External Event Edge Selection
                    | TC_CMR_EEVT_XC0     // External Event Source
                    //| TC_CMR_ENETRG                        // External Event Enable
                    | TC_CMR_WAVSEL_UP_RC // Waveform Selection, UP_RC resets TC_CV upon hitting RC.
                    | TC_CMR_WAVE // Wave select
                    | TC_CMR_ACPA_CLEAR       // Clears TIOA upon hitting RA
                    | TC_CMR_ACPC_SET        // Sets TIOA upon hitting RC
                    | TC_CMR_AEEVT_NONE
                    | TC_CMR_ASWTRG_NONE
                    | TC_CMR_BCPB_NONE
                    | TC_CMR_BCPC_NONE
                    | TC_CMR_BEEVT_NONE
                    | TC_CMR_BSWTRG_NONE
    );

    REG_TC0_CCR0 = TC_CCR_CLKEN | TC_CCR_SWTRG;

    REG_TC0_RC0 = RC * scalar;

    enable_write_protection();
}