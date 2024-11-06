#include "can.h"
#include "mcp.h"

void can_init() {
    mcp_init();
    mcp_write(MCP_RXB0CTRL, 0x60); // Enable receive buffer 0, higher priority buffer. Turn mask/filters off
    mcp_write(MCP_RXB1CTRL, 0x60); // Enable receive buffer 1, lower priority buffer.
    mcp_write(MCP_CANINTE, 0x00); // Disable all interrupts
    mcp_write(MCP_CANINTF, 0x00);

    mcp_write(MCP_CNF1, 0b10000011); // Set baud/bit rate to 125 kbps
    mcp_write(MCP_CNF2, 0b10110001);
    mcp_write(MCP_CNF3, 0b00000101);
    mcp_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);

}

void can_transmit(CanMessage* message) {
    mcp_write(TXB0SIDH, (message->id >> 3) & 0xFF); // Standard identifier high
    mcp_write(TXB0SIDL, (message->id & 0b111) << 5); // Standard identifier low
    mcp_write(TXB0DLC, message->length);
    for (int i = 0; i < message->length; i++) {
        mcp_write(TXB0D0 + i, message->data[i]);
    }
    mcp_rts(MCP_RTS_TX0);
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

bool can_receive(CanMessage* message) {
    if (!(mcp_read_status() & (1 << MCP_CANINTF_RX0IF))) {
        return false;
    }

    message->length = mcp_read(RXB0DLC);
    message->id = ((uint16_t)mcp_read(RXB0SIDH)) << 3;
    message->id |= ((uint16_t)mcp_read(RXB0SIDL) >> 5) & 0b111;

    for (int i = 0; i < message->length; i++) {
        message->data[i] = mcp_read(RXB0D0 + i);
    }

    mcp_bit_modify(MCP_CANINTF, (1 << MCP_CANINTF_RX0IF), 0 << MCP_CANINTF_RX0IF);

    printf("Received message: %c\n\r", message->data[0]);

    return true;
}

void handle_message_based_on_id(CanMessage* message) {
    switch (message->id)
    {
    case 0x01: // U Ded ID
        printf("Received message with id 0x01\n\r");
        printf("Game over\n\r");
        break;

    case 0x02: // Hit ID
        printf("Received message with id 0x02\n\r");
        printf("You got hit: You have %d lived left.\n\r", message->data[0]);
        break;
    }
}