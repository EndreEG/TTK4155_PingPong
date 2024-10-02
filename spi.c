#include "spi.h"

void spi_init(){
    set_bit(DDRB, DDB5); // Set MOSI output on PB5
    set_bit(DDRB, DDB7); // Set SCK output PB7
    clear_bit(DDRB, DDB6); // Set MISO input on PB6
    set_bit(DDRB, DDB4); // Set SS output on PB4
    // Enable SPI, Master, set clock rate fck/16
    set_bit(SPCR, SPE);
    set_bit(SPCR, MSTR);
    set_bit(SPCR, SPR0);
}

uint8_t spi_transceive(uint8_t data){
    // Start transmission
    SPDR = data;
    // Wait for transmission complete
    while(!(SPSR & (1<<SPIF)));

    data = SPDR;
    return data;
}

uint8_t mcp_read(uint8_t address){
    clear_bit(PORTB, PB4);
    spi_transceive(MCP_READ);
    spi_transceive(address);
    uint8_t data = spi_transceive(0x00);
    set_bit(PORTB, PB4);
    return data;
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

// char spi_receive(){
//     // Wait for reception complete
//     while(!(SPSR & (1<<SPIF)));
//     // Return Data Register
//     return SPDR;
// }