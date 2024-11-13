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

    bool game_running = false;
    CanMessage message;
    // can_construct_message(&message, 0x01, "DEAD");
    // can_transmit(message);
    can_construct_message(&message, 0x100, "ffD");
    can_transmit(message);

    while(1) {
        if (can_receive(&message)) {
            handle_message_based_on_id(&message, &game_running);
        }
        if (game_running) {
            run_game();
            game_running = false;
        }
    }
    
}