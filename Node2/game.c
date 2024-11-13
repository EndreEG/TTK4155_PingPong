#include <stdint.h>
#include <stdbool.h>
#include "ir.h"
#include "motor.h"
#include "game.h"
#include "can.h"
#include "box_interface.h"

void run_game() {
    printf("Game is running\n\r");
    CanMessage message;

    bool hit = false;
    uint16_t health = 10;
    bool _game_running = true;

    while (health > 0) {

        update_hit_status(&hit, &health, &message);
        
        if(!can_receive(&message)) {
            // printf("No message received\n\r");
        }

        else {
            handle_message_based_on_id(&message, &_game_running);
        }
    }
    PI_controller(0, 0, LEFT);
    printf("Game over\n\r");
}