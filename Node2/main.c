#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "can.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "uart.h"
#include "time.h"
#include "utilities.h"

int main()
{
    SystemInit();

    //Disable Watchdog Timer
    WDT->WDT_MR = WDT_MR_WDDIS; 

    uart_init(F_CPU, BAUDRATE);
    CanInit init = {
        .brp = 41,
        .phase1 = 6,
        .phase2 = 5,
        .propag = 1,
        .sjw = 3,
        .smp = 0
    };
    can_init(init, 0);


    // Enable the peripheral clock for PIOB
    set_bit(PMC->PMC_PCER0, ID_PIOB);


    // Configure PB13 as an output
    PIOB->PIO_OER = PIO_PB13;

    // CanMessage message;
    // can_transmit(message);
    // time_spinFor(msecs(1000));
    CanMessage message;
	can_construct_message(&message, 4, "LOL123");
    can_transmit(&message);
    // message.id = 0;
    // message.length = 0;
    // for (int i = 0; i < 8; i++) {
    //     message.data[i] = 0;
    // }
    while(1) {}
    
    // while (1)
    // {
    //     if(!can_receive(&message)) {
    //         printf("Waiting for message\n\r");
    //     }
    //     else {
    //         printf("Received message\n\r");
    //         can_print_message(&message);
    //         time_spinFor(msecs(1000));
    //     }
    // //     // can_print_message(&message);
    // //     // printf("Hello World\n\r");
    // //     // PIOB->PIO_CODR = PIO_PB13;
    // //     // time_spinFor(msecs(2500));
    // //     // PIOB->PIO_SODR = PIO_PB13;
    // //     // time_spinFor(msecs(500));
    // }
    
}