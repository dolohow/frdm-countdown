#include "MKL46Z4.h"

#include "rtc.h"
#include "timer.h"
#include "main.h"
#include "lcd.h"

void rtc_init()
{
    /* Enable clock */
    SIM->SCGC6 |= SIM_SCGC6_RTC_MASK;

    /* Ensure PTC1 is configured as RTC input clock */
    PORTC->PCR[1] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[1] = PORT_PCR_MUX(1);

    /* Clear the OSC32KSEL field to select osc */
    SIM->SOPT1 &= ~SIM_SOPT1_OSC32KSEL_MASK;
    /* RTC_CLKIN selected as the ERCLK32K */
    SIM->SOPT1 |= SIM_SOPT1_OSC32KSEL(2);

    /* Set the oscillator enable bit */
    OSC0->CR |= OSC_CR_ERCLKEN_MASK;
    /* Enable interrupt */
    NVIC_ClearPendingIRQ(RTC_Seconds_IRQn);
    NVIC_EnableIRQ(RTC_Seconds_IRQn);
    NVIC_SetPriority(RTC_Seconds_IRQn, 2);

    /* Time seconds interrupt enable */
    RTC->IER |= RTC_IER_TSIE_MASK;

    /* Enable time counter */
    RTC->TSR =  0;
    RTC->SR |= RTC_SR_TCE_MASK;
}

void RTC_Seconds_IRQHandler()
{
    if (is_started) {
        decrement_time(&min, &sec, 1);
        lcd_displaytime(min, sec);
    }
}
