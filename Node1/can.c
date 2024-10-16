#include "can.h"
#include "mcp.h"

void can_init() {
    mcp_init();
    mcp_write(MCP_RXB0CTRL, 0x60); // Enable receive buffer 0, higher priority buffer
    mcp_write(MCP_RXB1CTRL, 0x60); // Enable receive buffer 1, lower priority buffer.
    mcp_write(MCP_CANINTE, 0x03);
    mcp_write(MCP_CANINTF, 0x00);
    mcp_bit_modify(MCP_CNF1, 0b01001011, 0xFF); // Set baud/bit rate to 125 kbps
    mcp_bit_modify(MCP_CNF2, 0b11011011, 0xFF);
    mcp_bit_modify(MCP_CNF3, 0b00000101, 0xFF);

    mcp_write(MCP_CANCTRL, MODE_LOOPBACK);
}

void can_transmit(CanMessage* message) {
    mcp_write(TXB0SIDH, message->id >> 8); // Standard identifier high
    mcp_write(TXB0SIDL, message->id & 0xFF); // Standard identifier low
    mcp_write(TXB0DLC, message->length);
    for (int i = 0; i < message->length; i++) {
        mcp_write(TXB0D0 + i, message->data[i]);
    }
    mcp_rts(MCP_RTS_TX0);
    printf("Transmitted message: %c\n\r", message->data[0]);
}

void can_construct_message(CanMessage* message, uint16_t id, uint8_t* data) {
    message->id = id;
    message->length = MESSAGE_LENGTH;
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message->data[i] = data[i];
    }
}

void can_print_message(CanMessage* message) {
    printf("ID: %x\n\r", message->id);
    uint8_t i = 0;
    while(message->data[i] != '\0'){
        printf("%c", message->data[i]);
        i++;
    }
    printf("\n\r");
}

CanMessage can_receive(){
    CanMessage message;
    message.id = mcp_read(RXB0SIDH) << 8;
    message.id |= mcp_read(RXB0SIDL);
    message.length = mcp_read(RXB0DLC) & 0x0F;
    for (int i = 0; i < message.length; i++) {
        message.data[i] = mcp_read(RXB0D0 + i);
    }
    printf("Received message: %c\n\r", message.data[0]);
    return message;   
}