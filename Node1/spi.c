#include "spi.h"
void set_ss_low() {
    clear_bit(PORTB, PB4);
}

void set_ss_high() {
    set_bit(PORTB, PB4);
}


void spi_init(){
    set_bit(DDRB, DDB5); // Set MOSI output on PB5
    set_bit(DDRB, DDB7); // Set SCK output PB7

    clear_bit(DDRB, DDB6); // Set MISO input on PB6
    clear_bit(PORTB, PB6); // Disable pull-up resistor on MISO, Just in case
    set_bit(DDRB, DDB4); // Set SS output on PB4
    set_ss_high(); // Set SS high to disable slave during initialization

    // Enable SPI, Master, set clock rate fck/16
    set_bit(SPCR, SPE); // Enable SPI
    set_bit(SPCR, MSTR); // Set as master
    set_bit(SPCR, SPR0); // Set clock rate fck/16

    // Set SPI mode to 0
    clear_bit(SPCR, CPOL); // Set clock polarity to 0
    clear_bit(SPCR, CPHA); // Set clock phase to 0
    
    // set_bit(SPCR, SPIE); // Enable SPI interrupt
}

uint8_t spi_transceive(uint8_t data){
    // Start transmission
    SPDR = data;

    // Wait for transmission complete
    while(!(SPSR & (1<<SPIF))) {}

    uint8_t result = SPDR;
    return result;
}

uint8_t spi_read() 
{
    uint8_t dummy = 0xFF;
    SPDR = dummy;
    while(!(SPSR & (1<<SPIF)));
    return SPDR;
}

void spi_write(uint8_t data) 
{
    SPDR = data;
    while(!(SPSR & (1<<SPIF)));
}