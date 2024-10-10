#include "MCP2515.h"
#include "SPI.h"

#include "../gpio.h"

const uint8_t RESET = (uint8_t) 0b11000000;
const uint8_t READ = (uint8_t) 0b00000011;
const uint8_t READ_RX_BUFFER = (uint8_t) 0b10010000; // bit 1 and 2 variable, not bit 0.
const uint8_t WRITE = (uint8_t) 0b00000010;
const uint8_t LOAD_TX_BUFFER = (uint8_t) 0b01000000; // 3 last bits variable
const uint8_t REQUEST_TO_SEND = (uint8_t) 0b10000000; // 3 last bits variable
const uint8_t READ_STATUS = (uint8_t) 0b10100000;
const uint8_t RX_STATUS = (uint8_t) 0b10110000;
const uint8_t BIT_MODIFY = (uint8_t) 0b00000101;

const uint8_t TXRTSCTRL = (uint8_t) 0x0D;
const uint8_t CNF1 = (uint8_t) 0x2A;
const uint8_t CNF2 = (uint8_t) 0x29;
const uint8_t CNF3 = (uint8_t) 0x28;

const uint8_t EFLG = (uint8_t) 0x2D;

void Select_SPI(){
    write_pin(B, 4, 0);
}

void Unselect_SPI(){
    write_pin(B, 4, 1);
}

uint8_t read(uint8_t address){
    Select_SPI();

    transmit_spi(READ);
    transmit_spi(address);
    uint8_t data_from_mcp = receive_spi();

    Unselect_SPI();
    
    return data_from_mcp;
}

void write(uint8_t address, uint8_t data){
    Select_SPI();

    transmit_spi(WRITE);
    transmit_spi(address);
    transmit_spi(data);

    Unselect_SPI();
}

void request_to_send(){
    Select_SPI();

    transmit_spi(REQUEST_TO_SEND);

    Unselect_SPI();
}

uint8_t read_status(){
    Select_SPI();

    transmit_spi(READ_STATUS);
    uint8_t status = receive_spi();

    Unselect_SPI();

    return status;
    // Status bit representation:
    // 0: CANINTF.RC0IF
    // 1: CANINTFL.RX1IF
    // 2: TXB0CNTRL.TXREQ
    // 3: CANINTF.TX0IF
    // 4: TXB1CNTRL.TXREQ
    // 5: CANINTF.TX1IF
    // 6: TXB2CNTRL.TXREQ
    // 7: CANINTF.TX2IF
}

void bit_modify(uint8_t address, uint8_t mask_byte, uint8_t data){
    // *Address = (*Address & ~mask_byte) | (data & mask_byte);

    Select_SPI();

    transmit_spi(BIT_MODIFY);
    transmit_spi(address);
    transmit_spi(mask_byte);
    transmit_spi(data);

    Unselect_SPI();
}

void reset(){
    Select_SPI();

    transmit_spi(RESET);

    Unselect_SPI();
}

void init_MCP2515(){
    init_spi(); // Set Slave Select to output, B4
    Unselect_SPI();

    reset();
}