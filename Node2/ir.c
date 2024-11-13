#include <stdint.h>
#include <stdbool.h>
#include "utilities.h"
#include "sam.h"
#include "ir.h"
#include "adc.h"
#include "can.h"

bool ir_hit()
{
    uint16_t adc_value = adc_read();
    return adc_value < 1000;
}

void update_hit_status(bool *hit, uint16_t *health, CanMessage *message)
{
    if (ir_hit()) {
        if (!(*hit)) {
            *hit = true;
            (*health)--;
            printf("You got hit: You have %d lives left.\n\r", *health);
            if (*health == 0) {
                can_construct_message(message, 0x01, "DEAD");
                can_transmit(*message);
            }
            else {
                can_construct_message(message, 0x02, *health);
                can_transmit(*message);
            }
        }
    }
    else {
        *hit = false;
    }
}
