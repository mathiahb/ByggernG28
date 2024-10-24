#include "stdint.h"

// Takes value between 0 and 1000. 
// promille = D*1000
void set_duty_cycle_A(uint32_t promille);

void init_timercounter();