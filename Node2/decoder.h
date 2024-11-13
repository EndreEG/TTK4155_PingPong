#pragma once

#include <stdint.h>
#include "sam.h"

#define MAX_DECODER_VALUE 5600.0f
#define MIN_DECODER_VALUE 0

void decoder_init();
float decoder_read();