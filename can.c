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

void can_transmit(can_message* message) {
    mcp_write(TXB0SIDH, message->id >> 8);
    mcp_write(TXB0SIDL, message->id & 0xFF);
    mcp_write(TXB0DLC, message->length);
    for (int i = 0; i < message->length; i++) {
        mcp_write(TXB0D0 + i, message->data[i]);
    }
    mcp_rts(MCP_RTS_TX0);
}

can_message can_receive(){
    can_message message;
    message.id = mcp_read(RXB0SIDH) << 8;
    message.id |= mcp_read(RXB0SIDL);
    message.length = mcp_read(RXB0DLC) & 0x0F;
    for (int i = 0; i < message.length; i++) {
        message.data[i] = mcp_read(RXB0D0 + i);
    }
    return message;   
}