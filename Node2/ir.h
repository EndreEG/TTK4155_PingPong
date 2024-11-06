#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "can.h"

bool ir_hit();
void update_hit_status(bool* hit, uint16_t* health, CanMessage* message);