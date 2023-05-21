
#ifndef LEDTAPE_UTILS_H
#define LEDTAPE_UTILS_H

#include <pio.h>
#include "target.h"
#include "pacer.h"
#include "ledtape_utils.h"
#include "ledbuffer.h"

void set_pattern(int *count, ledbuffer_t *leds, bool *blue);

void set_pattern_simple(uint8_t *leds);

#endif /* LEDTAPE_UTILS_H */