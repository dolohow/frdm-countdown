#ifndef TIMER_H
#define TIMER_H

#include "MKL46Z4.h"

void increment_time(uint8_t *min, uint8_t *sec, uint8_t val);
void decrement_time(uint8_t *min, uint8_t *sec, uint8_t val);
void reset_time(uint8_t *min, uint8_t *sec);

#endif
