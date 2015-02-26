#include "MKL46Z4.h"

#include "leds.h"

void leds_init()
{
    /* Enable port clocks */
    SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;

    /* Configure PTD5 and PTE29 pin for its GPIO function */
    PORTD->PCR[5] = PORT_PCR_MUX(1);
    PORTE->PCR[29] = PORT_PCR_MUX(1);

    /* Don't blink on startup */
    FPTD->PSOR |= LED_RED_MASK;
    FPTE->PSOR |= LED_GREEN_MASK;

    /* Set pins as output */
    FPTD->PDDR |= LED_RED_MASK;
    FPTE->PDDR |= LED_GREEN_MASK;
}

void leds_toggle()
{
    /* Write to toggle register */
    FPTE->PTOR |= LED_GREEN_MASK;
    FPTD->PTOR |= LED_RED_MASK;
}

void leds_off()
{
    /* Write to clear register */
    FPTE->PSOR |= LED_GREEN_MASK;
    FPTD->PSOR |= LED_RED_MASK;
}
