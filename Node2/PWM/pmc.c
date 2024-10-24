#include "pmc.h"

#include "sam/sam3x/include/sam.h"

void init_PMC(){
    REG_PMC_WPMR &= ~(PMC_WPMR_WPEN);

    // Key = 0x37
    /*REG_CKGR_MOR = (0x37 << CKGR_MOR_KEY_Pos) | (0 << CKGR_MOR_MOSCXTST_Pos) | (CKGR_MOR_MOSCXTEN) | (CKGR_MOR_MOSCSEL);

    while(!(REG_PMC_SR & (PMC_SR_MOSCXTS)));

    REG_CKGR_PLLAR = (CKGR_PLLAR_ONE) | (1 << CKGR_PLLAR_MULA_Pos) | (0 << CKGR_PLLAR_PLLACOUNT_Pos) | (1 << CKGR_PLLAR_DIVA_Pos);
    
    while(!(REG_PMC_SR & (PMC_SR_LOCKA)));

    REG_PMC_MCKR = (PMC_MCKR_CSS_MAIN_CLK << PMC_MCKR_CSS_Pos);

    while(!(REG_PMC_SR & (PMC_SR_MCKRDY)));

    REG_PMC_MCKR = (PMC_MCKR_PRES_CLK_1);

    while(!(REG_PMC_SR & (PMC_SR_MCKRDY)));

    REG_PMC_PCK = PMC_PCK_CSS_MAIN_CLK | PMC_PCK_PRES_CLK_1;

    while(!(REG_PMC_SR & (PMC_SR_PCKRDY0)));*/

    REG_PMC_PCER0 = (PMC_PCER0_PID27);

    REG_PMC_WPMR |= ~(PMC_WPMR_WPEN);
}