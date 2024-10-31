#include "pmc.h"

#include "sam/sam3x/include/sam.h"

void init_PMC(){
    REG_PMC_WPMR &= ~(PMC_WPMR_WPEN);

    REG_PMC_PCER0 = (PMC_PCER0_PID27);
    REG_PMC_PCER1 = (PMC_PCER1_PID36);
    REG_PMC_PCER1 = (PMC_PCER1_PID37);

    REG_PMC_WPMR |= ~(PMC_WPMR_WPEN);
}