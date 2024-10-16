#pragma once
#include "utilities.h"

// Transmit buffer 0
#define TXB0SIDH 0b00110001 // Standard identifier high
#define TXB0SIDL 0b00110010 // Standard identifier low
#define TXB0DLC 0b00110101 // Data length code
#define TXB0D0 0b00110110 // Data byte 0

// Transmit buffer 1
#define TXB1SIDH 0b01000001
#define TXB1SIDL 0b01000010
#define TXB1DLC 0b01000101
#define TXB1D0 0b01000110

// Transmit buffer 2
#define TXB2SIDH 0b01010001
#define TXB2SIDL 0b01010010
#define TXB2DLC 0b01010101
#define TXB2D0 0b01010110

// Receive buffer 0
#define RXB0SIDH 0b01100001
#define RXB0SIDL 0b01100010
#define RXB0DLC 0b01100101
#define RXB0D0 0b01100110


// Receive buffer 1
#define RXB1SIDH 0b01110001
#define RXB1SIDL 0b01110010
#define RXB1DLC 0b01110101
#define RXB1D0 0b01110110



#define MESSAGE_LENGTH 8

typedef struct {
    uint16_t id;
    uint8_t length;
    uint8_t data[8];
} can_message;


void can_init();
void can_transmit(can_message* message);
void can_construct_message(can_message* message, uint16_t id, uint8_t* data);
void can_print_message(can_message* message);
can_message can_receive();