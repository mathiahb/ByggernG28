#include "adc.h"

#include "../sam/sam3x/include/sam.h"

volatile int score = 0;
volatile int mode_high = 1;
volatile int mode_capture = 0;

volatile int average = 0;
const int number_of_runs = 50;

void init_ADC(){
    REG_ADC_WPMR = (0x414443 << ADC_WPMR_WPKEY_Pos);

    REG_ADC_MR = ADC_MR_FREERUN_ON | ADC_MR_SETTLING_AST9;

    REG_ADC_CHER = ADC_CHER_CH10;

    REG_ADC_EMR = ADC_EMR_CMPMODE_HIGH | ADC_EMR_CMPSEL(10);
    REG_ADC_CWR = ADC_CWR_LOWTHRES(1000) | ADC_CWR_HIGHTHRES(3000);

    REG_ADC_IER = ADC_IER_COMPE;
    NVIC_EnableIRQ(ID_ADC);
    
    //REG_ADC_WPMR = ADC_WPMR_WPEN | (0x414443 << ADC_WPMR_WPKEY_Pos);

    REG_ADC_CR = ADC_CR_START; //| ADC_CR_SWRST; // ADC_CR_SWRST
}

void ADC_Handler(void){
    if(mode_high){
        REG_ADC_EMR = ADC_EMR_CMPMODE_LOW | ADC_EMR_CMPSEL(10);

        mode_high = !mode_high;

        REG_ADC_IDR = ADC_IDR_COMPE;

        REG_TC0_CCR1 = TC_CCR_SWTRG;
    }else{
        if(mode_capture == number_of_runs){
            if(average <= 1100){
                score += 1;
                printf("Score: %u\r\n", score);
            }

            average = 0;

            REG_ADC_EMR = ADC_EMR_CMPMODE_HIGH | ADC_EMR_CMPSEL(10);

            mode_high = !mode_high;

            mode_capture = 0;
            REG_ADC_IDR = ADC_IDR_EOC10;

            REG_ADC_IDR = ADC_IDR_COMPE;
            REG_TC0_CCR1 = TC_CCR_SWTRG;
        }else{
            average += (REG_ADC_LCDR & ADC_LCDR_LDATA_Msk) / number_of_runs;
            mode_capture += 1;
            REG_ADC_IER = ADC_IER_EOC10;
        }
    }

    //REG_ADC_CR = ADC_CR_START;

    uint32_t status = REG_ADC_ISR;
    NVIC_ClearPendingIRQ(ID_ADC);
}

void TC1_Handler(void){
    //printf("Goodbye.\r\n");
    REG_ADC_IER = ADC_IER_COMPE;

    uint32_t status = REG_TC0_SR1;
    NVIC_ClearPendingIRQ(ID_TC1);
}