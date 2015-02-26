#ifndef SLIDER_H
#define SLIDER_H

#include "MKL46Z4.h"

#define SLIDER_LENGTH 40
#define TOTAL_ELECTRODE 2

#define _TSI0 0
#define TSI1 1
#define TSI2 2
#define TSI3 3
#define TSI4 4
#define TSI5 5
#define TSI6 6
#define TSI7 7
#define TSI8 8
#define TSI9 9
#define TSI10 10
#define TSI11 11
#define TSI12 12
#define TSI13 13
#define TSI14 14
#define TSI15 15

/* Chose the correct TSI channel for the electrode number */
#ifdef FRDM_REVA
#define ELECTRODE0   TSI2
#define ELECTRODE1   TSI3
#else
#define ELECTRODE0   TSI9
#define ELECTRODE1   TSI10
#endif

#define ELECTRODE2   _TSI0
#define ELECTRODE3   TSI1
#define ELECTRODE4   TSI2
#define ELECTRODE5   TSI3
#define ELECTRODE6   TSI4
#define ELECTRODE7   TSI5
#define ELECTRODE8   TSI6
#define ELECTRODE9   TSI7
#define ELECTRODE10  TSI8
#define ELECTRODE11  TSI11
#define ELECTRODE12  TSI12
#define ELECTRODE13  TSI13
#define ELECTRODE14  TSI14
#define ELECTRODE15  TSI15

extern uint8_t total_electrode;
extern uint8_t elec_array[16];
extern uint16_t gu16TSICount[16];
extern uint16_t gu16Baseline[16];
extern uint16_t gu16Delta[16];
extern uint8_t ongoing_elec;
extern uint8_t AbsolutePercentegePosition;
extern uint8_t AbsoluteDistancePosition;

extern uint8_t end_flag;

void self_calibration(void);
void tsi_init(void);
void tsi_slider_read(void);
void change_electrode(void);

#endif
