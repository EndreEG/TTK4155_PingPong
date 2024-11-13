#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "can.h"
#include "adc.h"

#include "uart.h"
#include "time.h"
#include "pwm.h"
#include "motor.h"
#include "utilities.h"
#include "ir.h"

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
    pwm_init();
    adc_init();
    motor_init();
    solenoid_init();


    CanMessage message;
	can_construct_message(&message, 4, "LOL123");

    bool hit = false;
    uint16_t health = 100;
    while (1)
    {
        // fire_solenoid();
        // time_spinFor(msecs(1000));
        update_hit_status(&hit, &health, &message);
        
        if(!can_receive(&message)) {
            // printf("Waiting for message\n\r");
            // time_spinFor(msecs(100));
        }
        else {
            // can_print_message(&message);
            handle_message_based_on_id(&message);
            // time_spinFor(msecs(500));
        }
    //     // can_print_message(&message);
    //     // printf("Hello World\n\r");
    //     // PIOB->PIO_CODR = PIO_PB13;
    //     // time_spinFor(msecs(2500));
    //     // PIOB->PIO_SODR = PIO_PB13;
    //     // time_spinFor(msecs(500));
    }
    
}