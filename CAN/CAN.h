#include "stdint.h"

typedef struct
{
    uint16_t ID;
    uint8_t data_length; // 0-8 (number of bytes in data field)
    uint8_t data[8];
    uint8_t remote_frame;
} CAN_Message;


void init_CAN();

void CAN_transmit(CAN_Message message);
void CAN_interrupt_handler();