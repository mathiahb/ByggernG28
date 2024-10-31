#include "adc.h"

#include "../sam/sam3x/include/sam.h"

volatile int score = 0;
volatile int mode_high = 1;

void init_ADC(){
    REG_ADC_WPMR = (0x414443 << ADC_WPMR_WPKEY_Pos);

    REG_ADC_MR = ADC_MR_FREERUN_ON;

    REG_ADC_CHER = ADC_CHER_CH10;

    REG_ADC_EMR = ADC_EMR_CMPMODE_HIGH | ADC_EMR_CMPSEL(10);
    REG_ADC_CWR = ADC_CWR_LOWTHRES(700) | ADC_CWR_HIGHTHRES(1500);

    REG_ADC_IER = ADC_IER_COMPE;
    NVIC_EnableIRQ(ID_ADC);
    
    //REG_ADC_WPMR = ADC_WPMR_WPEN | (0x414443 << ADC_WPMR_WPKEY_Pos);

    REG_ADC_CR = ADC_CR_START; //| ADC_CR_SWRST; // ADC_CR_SWRST
}

void ADC_Handler(void){
    if(mode_high){
        REG_ADC_EMR = ADC_EMR_CMPMODE_LOW | ADC_EMR_CMPSEL(10);
    }else{
        score += 1;
        REG_ADC_EMR = ADC_EMR_CMPMODE_HIGH | ADC_EMR_CMPSEL(10);
    }

    mode_high = !mode_high;

    //REG_ADC_IDR = ADC_IDR_COMPE

    printf("Score: %u\r\n", score);

    REG_ADC_CR = ADC_CR_START;

    uint32_t status = REG_ADC_ISR;
    NVIC_ClearPendingIRQ(ID_ADC);
}