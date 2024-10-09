#include "mcp.h"

uint8_t mcp_init() {
    spi_init();
    // mcp_reset(); // Reset signal is physical button
    // Self-test
    mcp_reset();
    uint8_t value = mcp_read(MCP_CANSTAT);
    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\n\r");
        return 1;
    }
    return 0;
}

void mcp_reset() {
    clear_bit(PORTB, PB4); // Select MCP2515 (as slave) by
    spi_write(MCP_RESET);
    set_bit(PORTB, PB4); // Deselect MCP2515 by setting SS high
}

uint8_t mcp_read(uint8_t address){
    // uint8_t dummy_value;
    uint8_t result;
    clear_bit(PORTB, PB4); // Select MCP2515 (as slave) by setting SS low

    spi_write(MCP_READ);
    spi_write(address);
    result = spi_read();
    // dummy_value = spi_transceive(MCP_READ);
    // dummy_value = spi_transceive(address);
    // uint8_t data = spi_transceive(0x00); // Send dummy data to receive data from MCP2515
    set_bit(PORTB, PB4); // Deselect MCP2515 by setting SS high
    return result;
}

void mcp_write(uint8_t address, uint8_t data){
    clear_bit(PORTB, PB4);
    spi_transceive(MCP_WRITE);
    spi_transceive(address);
    spi_transceive(data);
    set_bit(PORTB, PB4);
}

void mcp_rts(uint8_t instruction) {
    clear_bit(PORTB, PB4);
    spi_transceive(instruction);
    set_bit(PORTB, PB4);
}

uint8_t mcp_read_status() {
    clear_bit(PORTB, PB4);
    spi_transceive(MCP_READ_STATUS);
    uint8_t can_status = spi_transceive(0x00);
    set_bit(PORTB, PB4);
    return can_status;
}