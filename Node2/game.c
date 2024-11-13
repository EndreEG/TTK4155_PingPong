#include <stdint.h>
#include <stdbool.h>
#include "ir.h"
#include "game.h"
#include "can.h"

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
    printf("Game over\n\r");
}