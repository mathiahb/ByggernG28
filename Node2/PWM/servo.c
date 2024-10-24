#include "timercounter.h"
#include "servo.h"


const uint32_t min_duty = 45;
const uint32_t max_duty = 105;

void joystick_set_servo_position(uint32_t joystick_position){
    
    uint32_t duty_cycle = 75 + (joystick_position * 30) / 100;
    
    if(duty_cycle < min_duty){
        duty_cycle = min_duty;
    }else if(duty_cycle > max_duty){
        duty_cycle = max_duty;
    }

    set_duty_cycle_A(duty_cycle);
}


void init_PWM(){
    init_timercounter();

    joystick_set_servo_position(0);
}