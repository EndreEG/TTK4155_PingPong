#include "mcp.h"

// TODO: MCP2515 misbehaves. Needs punishment- I mean, debugging. 
// TODO: MCP does not respond to SPI commands. MISO line is always low. MOSI seems to work fine. We can put bits in SPDR and they are transmitted.

uint8_t mcp_init() {
    spi_init();
    // mcp_reset(); // Reset signal is physical button
    mcp_reset();
    mcp_write(MCP_CANCTRL, MODE_CONFIG); // Set MCP2515 to configuration mode
    // Self-test
    _delay_ms(3);
    uint8_t value = mcp_read(MCP_CANSTAT);
    printf("Value: %d\n\r", value);

    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 is NOT in configuration mode after reset!\n\r");
        return 1;
    }
    printf("MCP2515 is in configuration mode after reset!\n\r");
    return 0;
}

void mcp_reset() {
    set_ss_low(); // Select MCP2515 (as slave) by
    spi_write(MCP_RESET);
    set_ss_high(); // Deselect MCP2515 by setting SS high
}

uint8_t mcp_read(uint8_t address){
    uint8_t result;
    set_ss_low(); // Select MCP2515 (as slave) by setting SS low
    spi_write(MCP_READ);
    spi_write(address);
    result = spi_read(); // Send dummy data to receive data from MCP2515
    set_ss_high(); // Deselect MCP2515 by setting SS high
    return result;
}

void mcp_write(uint8_t address, uint8_t data){
    set_ss_low();
    spi_write(MCP_WRITE);
    spi_write(address);
    spi_write(data);
    set_ss_high();
}

void mcp_rts(uint8_t instruction) {
    set_ss_low();
    spi_transceive(instruction);
    set_ss_high();
}

uint8_t mcp_read_status() {
    set_ss_low();
    spi_transceive(MCP_READ_STATUS);
    uint8_t can_status = spi_transceive(0x00);
    set_ss_high();
    return can_status;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	set_ss_low();
	spi_write(MCP_BIT_MODIFY); //Kommando for å endre en eller flere bit
	spi_write(address); //Adressen der vi vil endre en eller flere bit
	spi_write(mask); //Maskeringsbyte, se forklaring nedenfor
	spi_write(data); //Verdiene som biten(e) skal endres til
    set_ss_high();
}

void mcp_set_mode(uint8_t mode) {
	mcp_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}