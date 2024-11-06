#pragma once

#define DUTY_CYCLE_MIN 591
#define DUTY_CYCLE_CENTRE 985
#define DUTY_CYCLE_MAX 1378
#define CPRD 13125


#include <stdint.h>

void pwm_init();
void pwm_set_duty_cycle(uint32_t duty_cycle, uint32_t channel);