#include <stdio.h>
#include <stdarg.h>
#include "sam.h"
#include "can.h"
#include "adc.h"

#include "uart.h"
#include "time.h"
#include "pwm.h"
#include "motor.h"
#include "box_interface.h"
#include "utilities.h"
#include "ir.h"
#include "decoder.h"

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
    decoder_init();
    PI_controller_init();


    CanMessage message;
	can_construct_message(&message, 4, "LOL123");

    bool hit = false;
    uint16_t health = 100;
    float decoder_value = 0;
    while (1)
    {
        // decoder_value = decoder_read();
        // printf("Decoder value: %.6f\n\r", decoder_value);

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
    }
    
}