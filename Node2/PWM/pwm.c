#include "pwm.h"
#include "pmc.h"
#include "../ExternalInterface/gpio.h"

#include "../sam/sam3x/include/sam.h"

// Takes value between 0 and 1000. 
// promille = D*1000
void set_duty_cycle_CH1(uint32_t promille){
    REG_PWM_CDTY1 = (promille * 13125) / 1000;
}

void set_duty_cycle_CH0(uint32_t promille){
    REG_PWM_CDTY0 = (promille * 13125) / 1000;
}

void init_pwm(){
    init_PMC();

    setup_pin_as_peripheral(B, 13, CONTROLLER_B);
    setup_pin_as_peripheral(B, 12, CONTROLLER_B);

    REG_PWM_WPCR = 0x50574D << PWM_WPCR_WPKEY_Pos | 0xFC | PWM_WPCR_WPCMD(0);

    REG_PWM_CMR1 = PWM_CMR_CPRE_MCK_DIV_128 | PWM_CMR_CPOL;

    REG_PWM_CPRD1 = 13125;

    set_duty_cycle_CH1(75);

    REG_PWM_ENA = PWM_ENA_CHID1;

    REG_PWM_CMR0 = PWM_CMR_CPRE_MCK_DIV_128 | PWM_CMR_CPOL;

    REG_PWM_CPRD0 = 13125;

    set_duty_cycle_CH1(0);

    REG_PWM_ENA = PWM_ENA_CHID0;

    REG_PWM_WPCR = 0x50574D << PWM_WPCR_WPKEY_Pos | 0xFC | PWM_WPCR_WPCMD(1);   
}