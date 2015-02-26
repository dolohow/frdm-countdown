#include "MKL46Z4.h"

#include "main.h"
#include "lcd.h"
#include "buttons.h"
#include "timer.h"
#include "slider.h"
#include "rtc.h"
#include "leds.h"
#include "tpm.h"

uint8_t min;
uint8_t sec;
uint8_t is_started;

int main(void) {
    is_started = 0;
    lcd_init();
    buttons_init();
    leds_init();
    tpm_init();
    tsi_init();
    rtc_init();
    SystemCoreClockUpdate();
    reset_time(&min, &sec);
    lcd_displaytime(min, sec);
    while (1) {
        tsi_slider_read();
        if (AbsolutePercentegePosition > 0) {
            sec = AbsolutePercentegePosition / 1.67;
            lcd_displaytime(min, sec);
        }
    };
}
