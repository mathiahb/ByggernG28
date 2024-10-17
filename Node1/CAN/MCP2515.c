#include "MCP2515.h"
#include "SPI.h"
#include "mcp2515_common.h"

#include "stdio.h"

#include "../ExternalInterface/gpio.h"

void Select_SPI(){
    write_pin(B, 4, 0);
}

void Unselect_SPI(){
    write_pin(B, 4, 1);
}

uint8_t read(uint8_t address){
    Select_SPI();

    transmit_spi(MCP_READ);
    transmit_spi(address);
    uint8_t data_from_mcp = receive_spi();

    Unselect_SPI();
    
    return data_from_mcp;
}

void write(uint8_t address, uint8_t data){
    Select_SPI();

    transmit_spi(MCP_WRITE);
    transmit_spi(address);
    transmit_spi(data);

    Unselect_SPI();
}

void request_to_send_0(){
    Select_SPI();

    transmit_spi(MCP_RTS_TX0); // REQUEST_TO_SEND + (1 << 0)

    Unselect_SPI();
}

uint8_t read_status(){
    Select_SPI();

    transmit_spi(MCP_READ_STATUS);
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

    transmit_spi(MCP_BITMOD);
    transmit_spi(address);
    transmit_spi(mask_byte);
    transmit_spi(data);

    Unselect_SPI();
}

void reset(){
    Select_SPI();

    transmit_spi(MCP_RESET);

    Unselect_SPI();
}

void write_arbitration_0(uint16_t arbitration){
    uint8_t arbitration_high = (uint8_t) (arbitration >> 3) & 0xFF;
    uint8_t arbitration_low = (uint8_t) (arbitration & 7) << 5;

    write(MCP_TXB0CTRL + 1, arbitration_high);
    write(MCP_TXB0CTRL + 2, arbitration_low);
}

void write_data_0(uint8_t position ,uint8_t data){
    write(MCP_TXB0CTRL + 6 + position, data);
}

void write_data_control_0(uint8_t data_length, uint8_t remote_frame){
    write(MCP_TXB0CTRL + 5, (remote_frame << 6) + data_length);
}

// Returns extended_remote_frame << 6 + data_length (4 bits)
uint8_t read_data_control_0(){
    return read(MCP_RXB0SIDH + 4) & 0b01001111;
}

void read_data_0(uint8_t* data, uint8_t data_length){
    for(int i = 0; i < data_length; i++){
        data[i] = read(MCP_RXB0SIDH + 5 + i);
    }
}

// Returns remote frame << 15 + id (11 bits)
uint16_t read_arbitration_0(){
    uint16_t arbitration = 0;

    uint16_t arbitration_high = (uint16_t) read(MCP_RXB0SIDH);
    uint16_t arbitration_low = (uint16_t) read(MCP_RXB0SIDH + 1);

    arbitration = (arbitration_high << 3) + (arbitration_low >> 5) + ((arbitration_low & 0b00010000) << 11);

    return arbitration;
}

uint8_t is_currently_transmitting_0(){
    return read(MCP_TXB0CTRL) & (1 << 3);
}

void init_MCP2515(){
    init_spi(); // Set Slave Select to output, B4
    Unselect_SPI();

    reset();
}