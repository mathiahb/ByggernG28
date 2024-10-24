#include "gpio.h"

#include "../sam/sam3x/include/sam.h"

#include "stdint.h"

void disable_write_protection_port(PORT port){
    if(port == A){
        REG_PIOA_WPMR &= ~(PIO_WPMR_WPEN);
    }
    if(port == B){
        REG_PIOB_WPMR &= ~(PIO_WPMR_WPEN);
    }
    if(port == C){
        REG_PIOC_WPMR &= ~(PIO_WPMR_WPEN);
    }
    if(port == D){
        REG_PIOD_WPMR &= ~(PIO_WPMR_WPEN);
    }
}

void enable_write_protection_port(PORT port){
    if(port == A){
        REG_PIOA_WPMR |= (PIO_WPMR_WPEN);
    }
    if(port == B){
        REG_PIOB_WPMR |= (PIO_WPMR_WPEN);
    }
    if(port == C){
        REG_PIOC_WPMR |= (PIO_WPMR_WPEN);
    }
    if(port == D){
        REG_PIOD_WPMR |= (PIO_WPMR_WPEN);
    }
}

void enable_pin(PORT port, uint32_t pin){
    if(port == A){
        REG_PIOA_PER = (1 << pin);
    }
    if(port == B){
        REG_PIOB_PER = (1 << pin);
    }
    if(port == C){
        REG_PIOC_PER = (1 << pin);
    }
    if(port == D){
        REG_PIOD_PER = (1 << pin);
    }
}

void disable_pin(PORT port, uint32_t pin){
    if(port == A){
        REG_PIOA_PDR = (1 << pin);
    }
    if(port == B){
        REG_PIOB_PDR = (1 << pin);
    }
    if(port == C){
        REG_PIOC_PDR = (1 << pin);
    }
    if(port == D){
        REG_PIOD_PDR = (1 << pin);
    }
}

void enable_output_on_pin(PORT port, uint32_t pin){
    if(port == A){
        REG_PIOA_OER = 1 << pin;
    }
    if(port == B){
        REG_PIOB_OER = 1 << pin;
    }
    if(port == C){
        REG_PIOC_OER = 1 << pin;
    }
    if(port == D){
        REG_PIOD_OER = 1 << pin;
    }
}

void disable_output_on_pin(PORT port, uint32_t pin){
    if(port == A){
        REG_PIOA_ODR = 1 << pin;
    }
    if(port == B){
        REG_PIOB_ODR = 1 << pin;
    }
    if(port == C){
        REG_PIOC_ODR = 1 << pin;
    }
    if(port == D){
        REG_PIOD_ODR = 1 << pin;
    }
}

void set_pin_as_output(PORT port, uint32_t pin){
    if(port == A){
        REG_PIOA_OWER = 1 << pin;
    }
    if(port == B){
        REG_PIOB_OWER = 1 << pin;
    }
    if(port == C){
        REG_PIOC_OWER = 1 << pin;
    }
    if(port == D){
        REG_PIOD_OWER = 1 << pin;
    }
}

void disable_pin_as_output(PORT port, uint32_t pin){
    if(port == A){
        REG_PIOA_OWDR = 1 << pin;
    }
    if(port == B){
        REG_PIOB_OWDR = 1 << pin;
    }
    if(port == C){
        REG_PIOC_OWDR = 1 << pin;
    }
    if(port == D){
        REG_PIOD_OWDR = 1 << pin;
    }
}

void write_output(PORT port, uint32_t pin, uint32_t value){
    if(port == A){
        if(value){
            REG_PIOA_ODSR |= (1 << pin);
        }else{
            REG_PIOA_ODSR &= ~(1 << pin);
        }
    }
    if(port == B){
        if(value){
            REG_PIOB_ODSR |= (1 << pin);
        }else{
            REG_PIOB_ODSR &= ~(1 << pin);
        }
    }
    if(port == C){
        if(value){
            REG_PIOC_ODSR |= (1 << pin);
        }else{
            REG_PIOC_ODSR &= ~(1 << pin);
        }
    }
    if(port == D){
        if(value){
            REG_PIOD_ODSR |= (1 << pin);
        }else{
            REG_PIOD_ODSR &= ~(1 << pin);
        }
    }
}

void select_peripheral_controller(PORT port, uint32_t pin, CONTROLLER controller){
    if(port == A){
        if(controller == CONTROLLER_A){
            REG_PIOA_ABSR &= ~(1 << pin);
        }
        if(controller == CONTROLLER_B){
            REG_PIOA_ABSR |= (1 << pin);
        }
    }
    if(port == B){
        if(controller == CONTROLLER_A){
            REG_PIOB_ABSR &= ~(1 << pin);
        }
        if(controller == CONTROLLER_B){
            REG_PIOB_ABSR |= (1 << pin);
        }
    }
    if(port == C){
        if(controller == CONTROLLER_A){
            REG_PIOC_ABSR &= ~(1 << pin);
        }
        if(controller == CONTROLLER_B){
            REG_PIOC_ABSR |= (1 << pin);
        }
    }
    if(port == D){
        if(controller == CONTROLLER_A){
            REG_PIOD_ABSR &= ~(1 << pin);
        }
        if(controller == CONTROLLER_B){
            REG_PIOD_ABSR |= (1 << pin);
        }
    }
}

uint32_t read_pin(PORT port, uint32_t pin){
    return 0;
}

void setup_pin_as_peripheral(PORT port, uint32_t pin, CONTROLLER controller){
    disable_write_protection_port(port);

    select_peripheral_controller(port, pin, controller);
    
    disable_output_on_pin(port, pin);
    disable_pin(port, pin);
    disable_pin_as_output(port, pin);

    enable_output_on_pin(port, pin);

    enable_write_protection_port(port);
}

void setup_pin_as_output(PORT port, uint32_t pin, uint32_t starting_value){
    disable_write_protection_port(port);    // WPEN disabled

    disable_output_on_pin(port, pin);       // ODR

    enable_pin(port, pin);                  // PER

    set_pin_as_output(port, pin);           // OWER
    write_output(port, pin, starting_value);// ODSR
    enable_output_on_pin(port, pin);        // OER

    enable_write_protection_port(port);     // WPEN enabled
}