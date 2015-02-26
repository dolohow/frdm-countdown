# Countdown Timer
- - -
![kl46z]
## Requirements
* Freedom KL46Z board
* CMSIS libraries

## Prerequisites
Please use mbed OpenSDA from [here][1] to generate 32.768kHZ on `PTC1` (`RTC_CLKIN`).

## Compilation
You can use any compilator you want, but you need to obtain a copy of headers.
I also prepared so called import file `countdown.uvprojx` that you can use with *Keil µVision*.

## Basic usage
By sliding through `TSI` you can change seconds, while the `SW1` can increase minutes.
To start countdown simply press `SW2` button.
You will be informed of an end counting when diodes start blinking. Then pushing `SW2` button will switch them off.

## References
* TSI code based on samples provided by [Freescale](http://www.freescale.com/webapp/sps/site/prod_summary.jsp?code=FRDM-KL46Z&fpsp=1&tab=Design_Tools_Tab)
* LCD code based on [eeewiki.net](https://eewiki.net/download/attachments/37552139/Seg_LCD.c)

[1]: http://mbed.org/media/uploads/sam_grove/20140530_k20dx128_kl46z_if_opensda.s19
[kl46z]: http://www.premierfarnell.com/sites/default/files/attachments/Freescale%20Freedom%20KL46z.jpg