#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

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

    // Enable the peripheral clock for PIOB
    set_bit(PMC->PMC_PCER0, ID_PIOB);

    // Configure PB13 as an output
    PIOB->PIO_OER = PIO_PB13;


    while (1)
    {
        printf("Hello World\n\r");
        PIOB->PIO_CODR = PIO_PB13;
        time_spinFor(msecs(500));
        PIOB->PIO_SODR = PIO_PB13;
        time_spinFor(msecs(500));
    }
    
}