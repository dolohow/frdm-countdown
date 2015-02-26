#ifndef LEDS_H
#define LEDS_H

#define LED_RED_MASK   1UL << 5
#define LED_GREEN_MASK 1UL << 29

void leds_init(void);
void leds_toggle(void);
void leds_off(void);

#endif
