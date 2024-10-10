#include "stdint.h"

void init_MCP2515();

uint8_t read(uint8_t address);
void write(uint8_t address, uint8_t data);
void request_to_send();
