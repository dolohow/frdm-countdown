#include "MKL46Z4.h"

#include "slider.h"

uint8_t total_electrode = TOTAL_ELECTRODE;
uint8_t elec_array[16]= {
    ELECTRODE0, ELECTRODE1, ELECTRODE2, ELECTRODE3, ELECTRODE4, ELECTRODE5,
    ELECTRODE6, ELECTRODE7, ELECTRODE8, ELECTRODE9, ELECTRODE10, ELECTRODE11,
    ELECTRODE12, ELECTRODE13, ELECTRODE14, ELECTRODE15
};
uint16_t gu16TSICount[16];
uint16_t gu16Baseline[16];
uint16_t gu16Delta[16];
uint8_t ongoing_elec;

uint8_t end_flag = 1;

uint8_t SliderPercentegePosition[2] = {0, 0};
uint8_t SliderDistancePosition[2] = {0, 0};


uint8_t AbsolutePercentegePosition = 0;
uint8_t AbsoluteDistancePosition = 0;

void TSI0_IRQHandler()
{
    end_flag = 1;
    /* Clear End of Scan Flag */
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
    change_electrode();
}

void change_electrode()
{
    int16_t u16temp_delta;
    /* Save Counts for current electrode */
    gu16TSICount[ongoing_elec] = (TSI0->DATA & TSI_DATA_TSICNT_MASK);
    /* Obtains Counts Delta from callibration reference */
    u16temp_delta = gu16TSICount[ongoing_elec] - gu16Baseline[ongoing_elec];
    if (u16temp_delta < 0) {
        gu16Delta[ongoing_elec] = 0;
    }
    else {
        gu16Delta[ongoing_elec] = u16temp_delta;
    }
    /* Change Electrode to scan */
    if (total_electrode > 1)
    {
        if ((total_electrode-1) > ongoing_elec) {
            ongoing_elec++;
        }
        else {
            ongoing_elec = 0;
        }
        TSI0->DATA = ((elec_array[ongoing_elec]<<TSI_DATA_TSICH_SHIFT));
        TSI0->DATA |= TSI_DATA_SWTS_MASK;
    }
}


void self_calibration()
{
    unsigned char cnt;
    unsigned char trigger_backup;
    /* Obtains Counts Delta from callibration reference */
    TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
    /* Disable TSI module */
    TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK;

    /* Back-up TSI Trigger mode from Application */
    if (TSI0->GENCS & TSI_GENCS_STM_MASK)
        trigger_backup = 1;
    else
        trigger_backup = 0;

    /* Use SW trigger */
    TSI0->GENCS &= ~TSI_GENCS_STM_MASK;
    /* Enable TSI interrupts */
    TSI0->GENCS &= ~TSI_GENCS_TSIIEN_MASK;

    /* Enable TSI module */
    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;

    /* Get Counts when Electrode not pressed */
    for(cnt=0; cnt < total_electrode; cnt++)
    {
        TSI0->DATA = ((elec_array[cnt] << TSI_DATA_TSICH_SHIFT) );
        TSI0->DATA |= TSI_DATA_SWTS_MASK;
        while(!(TSI0->GENCS & TSI_GENCS_EOSF_MASK));
        TSI0->GENCS |= TSI_GENCS_EOSF_MASK;
        gu16Baseline[cnt] = (TSI0->DATA & TSI_DATA_TSICNT_MASK);
    }

    /* Disable TSI module */
    TSI0->GENCS &= ~TSI_GENCS_TSIEN_MASK;
    /* Enale TSI interrupt */
    TSI0->GENCS |= TSI_GENCS_TSIIEN_MASK;
    /* Restore trigger mode */
    if (trigger_backup)
        TSI0->GENCS |= TSI_GENCS_STM_MASK;
    else
        TSI0->GENCS &= ~TSI_GENCS_STM_MASK;

    /* Enable TSI module */
    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;

    TSI0->DATA = ((elec_array[0]<<TSI_DATA_TSICH_SHIFT));
    TSI0->DATA |= TSI_DATA_SWTS_MASK;
}

void tsi_init()
{
    /* Enable clock gating for TSI */
    SIM->SCGC5 |= SIM_SCGC5_TSI_MASK;

    /* Enable proper GPIO as TSI channels */
    SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;
    /* PTB16 as TSI channel 9 */
    PORTB->PCR[16] =  PORT_PCR_MUX(0);
    /* PTB17 as TSI channel 10 */
    PORTB->PCR[17] =  PORT_PCR_MUX(0);

    NVIC_ClearPendingIRQ(TSI0_IRQn);
    NVIC_EnableIRQ(TSI0_IRQn);
    NVIC_SetPriority(TSI0_IRQn, 0);

    /* enable end of scan interrupt */
    TSI0->GENCS |= TSI_GENCS_ESOR_MASK |
        /* enable capacitance sensing non noise mode */
        TSI_GENCS_MODE(0) |
        /* reference oscillator charge/discharge current value is 2uA */
        TSI_GENCS_REFCHRG(4) |
        /* setting appropriate oscillators voltage rails */
        TSI_GENCS_DVOLT(0) |
        /* electrode oscillator charge/discharge current value is 64uA */
        TSI_GENCS_EXTCHRG(7) |
        /* electrode oscillator frequency is divided by 16 */
        TSI_GENCS_PS(4) |
        /* 12 scans per each electrode */
        TSI_GENCS_NSCN(11) |
        /* enable TSI interrupt */
        TSI_GENCS_TSIIEN_MASK |
        TSI_GENCS_STPE_MASK;
    /* Clear End of scan and Out of Range Flags */
    TSI0->GENCS |= TSI_GENCS_OUTRGF_MASK | TSI_GENCS_EOSF_MASK;
    /* Select Desired Channel to Scan */
    /* Choose channel 9 */
    TSI0->DATA |= TSI_DATA_TSICH(9);

    /* enable TSI */
    TSI0->GENCS |= TSI_GENCS_TSIEN_MASK;

    self_calibration();
}

void tsi_slider_read()
{
    if (end_flag) {
        end_flag = 0;
        if ((gu16Delta[0] > 100) || (gu16Delta[1] > 100)) {
            SliderPercentegePosition[0] =
                (gu16Delta[0]*100)/(gu16Delta[0]+gu16Delta[1]);

            SliderPercentegePosition[1] =
                (gu16Delta[1]*100)/(gu16Delta[0]+gu16Delta[1]);

            SliderDistancePosition[0] =
                (SliderPercentegePosition[0]* SLIDER_LENGTH)/100;

            SliderDistancePosition[1] =
                (SliderPercentegePosition[1]* SLIDER_LENGTH)/100;

            AbsolutePercentegePosition =
                ((100 - SliderPercentegePosition[0]) + SliderPercentegePosition[1])/2;
            AbsoluteDistancePosition =
                ((SLIDER_LENGTH - SliderDistancePosition[0]) + SliderDistancePosition[1])/2;

        }
        else {
            SliderPercentegePosition[0] = 0;
            SliderPercentegePosition[1] = 0;
            SliderDistancePosition[0] = 0;
            SliderDistancePosition[1] = 0;
            AbsolutePercentegePosition = 0;
            AbsoluteDistancePosition = 0;
        }
    }

}

