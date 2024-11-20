#include <stdint.h>
#include <stdbool.h>
#include "utilities.h"
#include "sam.h"
#include "ir.h"
#include "adc.h"
#include "can.h"

#define GAME_OVER 0x01
#define HIT 0x02

bool ir_hit()
{
    uint16_t adc_value = adc_read();
    printf("ADC value: %d\n\r", adc_value);
    return adc_value < 600; // 600 is the threshold for a hit, previously we used a higher number
}

void update_hit_status(bool *hit, uint16_t *health, CanMessage *message)
{
    if (ir_hit()) {
        if (!(*hit) && *health > 0) {
            *hit = true;
            (*health)--;
            printf("You got hit: You have %d lives left.\n\r", *health);
            if (*health == 0) {
                printf("Game over - U DEAD\n\r");
                can_construct_message(message, GAME_OVER, "DEAD");
                can_transmit(*message);
            }
            else {
                printf("Sending hit message\n\r");
                can_construct_message(message, HIT, *health);
                can_transmit(*message);
            }
        }
    }
    else {
        *hit = false;
    }
}
