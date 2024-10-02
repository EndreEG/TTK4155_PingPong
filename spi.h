#pragma once
#include "utilities.h"

#define MCP_READ 0b00000011
#define MCP_WRITE 0b00000010
#define MCP_RESET 0b11000000
#define MCP_READ_STATUS 0b10100000
#define MCP_BIT_MODIFY 0b00000101

void spi_init();
uint8_t spi_transceive(uint8_t data);
uint8_t mcp_read(uint8_t address);
void mcp_write(uint8_t address, uint8_t data);
void mcp_rts(uint8_t instruction);
uint8_t mcp_read_status();