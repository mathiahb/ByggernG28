#include "CAN.h"
#include "MCP2515.h"
#include "mcp2515_common.h"

#include "../../CAN_IDs.h"

#include "stdio.h"

#define F_MCP2515 16000000

void init_CAN(){
    init_MCP2515();

    write(MCP_CANCTRL, 4 << 5); // 4: Configuration Mode, 2: Loopback mode, 0: Normal mode

    uint8_t CNF3 = (PHASE2 << 0);
    uint8_t CNF2 = (1 << 7) | (1 << 6) | ((PHASE1) << 3) | ((PROPAG) << 0);
    uint8_t CNF1 = (SJW << 6) | (F_MCP2515 / BAUDRATE / 2 - 1);

    write(MCP_CNF3, CNF3);
    uint8_t cnf3 = read(MCP_CNF3);
    write(MCP_CNF2, CNF2);
    uint8_t cnf2 = read(MCP_CNF2);
    write(MCP_CNF1, CNF1);
    uint8_t cnf1 = read(MCP_CNF1);

    printf("1: %d - %d\r\n", cnf1, CNF1);
    printf("2: %d - %d\r\n", cnf2, CNF2);
    printf("3: %d - %d\r\n", cnf3, CNF3);

    write(MCP_CANCTRL, 0 << 5); // 2: Loopback mode, 0: Normal mode
    bit_modify(MCP_CANINTE, 1 << 0, 0xFF); // Set RX0IE


    write(MCP_RXM0SIDH, 0);
    write(MCP_RXM0SIDL, 0);
}

void CAN_transmit(CAN_Message message){
    while(is_currently_transmitting_0()){
        uint8_t transmit_error_count = read(0x1C);
        uint8_t read_error_count = read(0x1D);
        printf("Write error: %u, Read error: %u\r\n", transmit_error_count, read_error_count);
    } // Wait until not transmitting

    write_arbitration_0(message.ID);
    write_data_control_0(message.data_length, message.remote_frame);
    for(int i = 0; i < message.data_length; i++){
        write_data_0(i, message.data[i]);
    }

    request_to_send_0();
}

CAN_Message CAN_read(){
    CAN_Message received_message = {.ID = 0, .data_length = 0, .remote_frame = 0, .data = {0, 0, 0, 0, 0, 0, 0, 0}};

    uint16_t arbitration = read_arbitration_0();
    uint8_t data_length = read_data_control_0();
    

    received_message.ID = arbitration & 0x07FF;
    received_message.data_length = data_length & 0x0F;

    read_data_0(received_message.data, received_message.data_length);

    received_message.remote_frame = (arbitration >> 15);
    
    return received_message;
}

void CAN_interrupt_handler(){
    uint8_t flags = 0;
    
    while(flags = read(MCP_CANINTF)){
        if(flags & (1 << 0)){
            CAN_Message data = CAN_read();

            printf("Arbitration: %d, Data_length: %d, Remote_Frame: %d\r\n", data.ID, data.data_length, data.remote_frame);
            
            for(int i = 0; i < data.data_length; i++){
                printf("Data %d: %d\r\n", i, data.data[i]);
            }

            // RX0IF
            bit_modify(MCP_CANINTF, 1 << 0, 0);
        }
        if(flags & (1 << 1)){
            printf("RX1IF ble aktivert?\r\n");
            // RX1IF
            bit_modify(MCP_CANINTF, 1 << 1, 0);
        }
        if(flags & (1 << 2)){
            printf("TX0IF ble aktivert?\r\n");
            // TX0IF
            bit_modify(MCP_CANINTF, 1 << 2, 0);
        }
        if(flags & (1 << 3)){
            printf("TX1IF ble aktivert?\r\n");
            // TX1IF
            bit_modify(MCP_CANINTF, 1 << 3, 0);
        }
        if(flags & (1 << 4)){
            printf("TX2IF ble aktivert?\r\n");
            // TX2IF
            bit_modify(MCP_CANINTF, 1 << 4, 0);
        }
        if(flags & (1 << 5)){
            printf("ERRIF ble aktivert?\r\n");
            // ERRIF
            bit_modify(MCP_CANINTF, 1 << 5, 0);
        }
        if(flags & (1 << 6)){
            printf("WAKIF ble aktivert?\r\n");
            // WAKIF
            bit_modify(MCP_CANINTF, 1 << 6, 0);
        }
        if(flags & (1 << 7)){
            printf("MERRF ble aktivert?\r\n");
            // MERRF
            bit_modify(MCP_CANINTF, 1 << 7, 0);
        }
    }
}