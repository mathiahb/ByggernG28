#include "motor.h"
#include "pwm.h"
#include "timercounter.h"
#include "../ExternalInterface/gpio.h"

#include "sam/sam3x/include/sam.h"

const int32_t regulator_division_constant = 10;
const int32_t Kp = 50;
const int32_t Ki = 10;
const int32_t Kd = 1000;
const uint32_t f = 50000; // T = 1/f

volatile uint32_t r_n = 0; // r_n max = 2800
volatile int32_t e_previous = 0;
volatile int32_t diskret_integral = 0; // Også kjent som: Sum

void set_motor_position_ref(int32_t indata, uint32_t is_slider){
    r_n = 1400 + (1400 * indata) / 100;

    if(is_slider){
        r_n = (indata * 2800) / 255;
    }
}

void joystick_set_motor_speed(int32_t joystick){
    if(joystick > 0){
        write_output(C, 23, 0);
    }else{
        write_output(C, 23, 1);
        joystick = -joystick;
    }

    if(joystick <= 2) joystick = 0; // Dead zone

    set_duty_cycle_CH0(joystick * 7);
}

void regulator_set_motor_speed(int32_t u){
    if(u > 0){
        write_output(C, 23, 0);
    }else{
        write_output(C, 23, 1);
        u = -u;
    }

    //if(u <= 20) u = 0; // Dead zone

    if(u > 1000) u = 1000;

    set_duty_cycle_CH0(u);
}

void init_motor(){
    setup_pin_as_output(C, 23, 1);

    // Drive motor into wall.
    // This is for no good reason except to calibrate it so that 0 is at the wall.
    set_duty_cycle_CH0(1000);
    
    volatile uint32_t sleep = 10000000;
    while(--sleep);
    
    set_duty_cycle_CH0(0);

    init_timercounter(f);
    set_motor_position_ref(0, 0);
    //init_pwm(); // Is called from servo.c


}

void TC0_Handler(void){
    // REGULATOR! :D
    int32_t y_n = (int32_t) REG_TC2_CV0;
    int32_t e_n = (int32_t) r_n - y_n;


    int32_t u_P = Kp * e_n;
    int32_t u_I = Ki * diskret_integral / (int32_t) f;
    int32_t u_D = Kd * (int32_t) f * (e_n - e_previous);

    int32_t u_n = (u_P + u_I + u_D) / regulator_division_constant;

    if(abs(e_n) < 20) u_n = 0;
    else if(u_n < 1000){
        diskret_integral += e_n;
    }

    //printf("%d %d\r\n", u_n, diskret_integral);
    
    regulator_set_motor_speed(u_n);
    e_previous = e_n;
    

    uint32_t _ = REG_TC0_SR0;
    NVIC_ClearPendingIRQ(ID_TC0);
}