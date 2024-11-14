#include "pwm.h"
#include "servo.h"


const uint32_t min_duty = 45;
const uint32_t max_duty = 105;

void joystick_set_servo_position(int32_t joystick_position, uint32_t is_slider){
    int32_t duty_cycle = 75 + (joystick_position * 30) / 100;
    
    if(is_slider){
        duty_cycle = min_duty + (joystick_position * 60) / 255;
    }
    
    if(duty_cycle < min_duty){
        duty_cycle = min_duty;
    }else if(duty_cycle > max_duty){
        duty_cycle = max_duty;
    }

    set_duty_cycle_CH1(duty_cycle);
}


void init_servo(){
    //init_timercounter();
    init_pwm();

    joystick_set_servo_position(0, 0);
}