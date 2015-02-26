#include "MKL46Z4.h"

#include "timer.h"

void increment_time(uint8_t *min, uint8_t *sec, uint8_t val)
{
    uint8_t m = *min;
    m += (*sec+val)/60;
    if (m <= 99) {
        *min = m;
    }
    *sec = (*sec+val)%60;
}


void decrement_time(uint8_t *min, uint8_t *sec, uint8_t val)
{
    if (*min != 0 || *sec != 0) {
        *sec = *sec-val>=0 ? *sec-val : 60-*sec-val;
        *min -= (*sec+val)/60;
    }
}

void reset_time(uint8_t *min, uint8_t *sec)
{
    *min = 0;
    *sec = 0;
}
