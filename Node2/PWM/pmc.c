#include "pmc.h"

#include "sam/sam3x/include/sam.h"

void init_PMC(){
    REG_PMC_WPMR &= ~(PMC_WPMR_WPEN);

    REG_PMC_PCER0 = (PMC_PCER0_PID27); // Timercounter TC0 (TC0 CH0)
    REG_PMC_PCER0 = (PMC_PCER0_PID28); // Timercoutner TC1 (TC0 CH1)
    //REG_PMC_PCER0 = (PMC_PCER0_PID29); // Timercounter TC2 (TC0 CH2)
    REG_PMC_PCER0 = (1 << ID_PIOC);
    REG_PMC_PCER1 = (PMC_PCER1_PID33); // Timercounter TC6 (TC2 CH0)
    REG_PMC_PCER1 = (PMC_PCER1_PID36); // PWM
    REG_PMC_PCER1 = (PMC_PCER1_PID37); // PWM

    REG_PMC_WPMR |= ~(PMC_WPMR_WPEN);
}