#include "stdint.h"

void init_MCP2515();

uint8_t read(uint8_t address);
uint8_t read_status();

void write(uint8_t address, uint8_t data);
void bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data);

uint8_t is_currently_transmitting_0();
void request_to_send_0();
void write_arbitration_0(uint16_t arbitration);
void write_data_0(uint8_t position ,uint8_t data);
void write_data_control_0(uint8_t data_length, uint8_t remote_frame);

uint16_t read_arbitration_0(); // Returns remote frame (SRR) << 15 + id (11 bits)
void read_data_0(uint8_t* data, uint8_t data_length);
uint8_t read_data_control_0(); // Returns extended_remote_frame (RTR) << 6 + data_length (4 bits)