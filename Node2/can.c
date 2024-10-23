
#pragma once

#include "sam.h"
#include "can.h"
#include "utilities.h"
#include "time.h"
#include <stdio.h>

void can_print_message(CanMessage* message){
    printf("CanMessage(id:%d, length:%d, data:{", message->id, message->length);
    // if(message->length){
    //     printf("%d", message->data[0]);
    // }
    for(uint8_t i = 0; i < message->length; i++){
        printf(", %d", message->data[i]);
    }
    printf("})\n\r");
}


#define txMailbox 0
#define rxMailbox 1


void can_init(CanInit init, uint8_t rxInterrupt){
    // Disable CAN
    CAN0->CAN_MR &= ~CAN_MR_CANEN; 
    
    // Clear status register by reading it
    __attribute__((unused)) uint32_t ul_status = CAN0->CAN_SR;     
    
    // Disable interrupts on CANH and CANL pins
    PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
    
    // Select CAN0 RX and TX in PIOA
    PIOA->PIO_ABSR &= ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);
    
    // Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral controller can use them
    PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;
    
    // Enable pull up on CANH and CANL pin
    PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);    
    
    // Enable Clock for CAN0 in PMC
    // DIV = 1 (can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
    PMC->PMC_PCR = PMC_PCR_EN | (0/*??*/ << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_CAN0 << PMC_PCR_PID_Pos); 
    PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);
    
    //Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match on all nodes!
    CAN0->CAN_BR = init.reg; 
    


    // Configure mailboxes
    // transmit
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDE;
    CAN0->CAN_MB[txMailbox].CAN_MMR = CAN_MMR_MOT_MB_TX;
    
    // receive
    CAN0->CAN_MB[rxMailbox].CAN_MAM = 0; // Accept all messages
    CAN0->CAN_MB[rxMailbox].CAN_MID = CAN_MID_MIDE;
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    if(rxInterrupt){
        // Enable interrupt on receive
        CAN0->CAN_IER |= (1 << rxMailbox); 
        // Enable interrupt in NVIC 
        NVIC_EnableIRQ(ID_CAN0);
    }

    // Enable CAN
    CAN0->CAN_MR |= CAN_MR_CANEN;
}

void can_construct_message(CanMessage* message, uint16_t id, uint8_t* data) {
    message->id = id;
    message->length = MESSAGE_LENGTH;
    for (int i = 0; i < MESSAGE_LENGTH; i++) {
        message->data[i] = data[i];
    }
}


void can_transmit(CanMessage message){
    while(!(CAN0->CAN_MB[txMailbox].CAN_MSR & CAN_MSR_MRDY)){}
    
    // Set message ID and use CAN 2.0B protocol
    CAN0->CAN_MB[txMailbox].CAN_MID = CAN_MID_MIDvA(message.id) | CAN_MID_MIDE ;
        
    // Coerce maximum 8 data length
    message.length = message.length > 8 ? 8 : message.length;
    
    //  Put message in can data registers
    CAN0->CAN_MB[txMailbox].CAN_MDL = message.dword[0];
    CAN0->CAN_MB[txMailbox].CAN_MDH = message.dword[1];
        
    // Set message length and mailbox ready to send
    CAN0->CAN_MB[txMailbox].CAN_MCR = (message.length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
}

uint8_t can_receive(CanMessage* message){
    if(!(CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MRDY)){
        return 0;
    }
    // time_spinFor(msecs(2000));

    // Get message ID
    message->id = (uint16_t)((CAN0->CAN_MB[rxMailbox].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
        
    // Get data length
    message->length = (uint8_t)((CAN0->CAN_MB[rxMailbox].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
    
    // Get data from CAN mailbox
    message->dword[0] = CAN0->CAN_MB[rxMailbox].CAN_MDL;
    message->dword[1] = CAN0->CAN_MB[rxMailbox].CAN_MDH;
                
    // Reset for new receive
    CAN0->CAN_MB[rxMailbox].CAN_MMR = CAN_MMR_MOT_MB_RX;
    CAN0->CAN_MB[rxMailbox].CAN_MCR |= CAN_MCR_MTCR;
    return 1;
}
    
    

    
/*
// Example CAN interrupt handler
void CAN0_Handler(void){
    char can_sr = CAN0->CAN_SR; 
    
    // RX interrupt
    if(can_sr & (1 << rxMailbox)){
        // Add your message-handling code here
        can_print_message(can_receive());
    } else {
        printf("CAN0 message arrived in non-used mailbox\n\r");
    }
    
    if(can_sr & CAN_SR_MB0){
        // Disable interrupt
        CAN0->CAN_IDR = CAN_IER_MB0;
    }
    
    NVIC_ClearPendingIRQ(ID_CAN0);
} 
*/

