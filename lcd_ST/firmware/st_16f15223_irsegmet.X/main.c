#include <stdint.h>
#include "header.h"

#define _XTAL_FREQ 32000000UL

#define AVG_SAMPLES       50
#define PID_PERIOD_MS     200
#define SET_DISPLAY_MS    3000


/* ================= GLOBAL VARIABLES ================= */

uint16_t avr_t[AVG_SAMPLES] = {0};

volatile uint8_t tick_1ms = 0;

uint16_t tick_1s = 0;
uint16_t tick_pid = 0;

uint8_t pid_en = 0;
uint8_t SW_Dp = 0;

uint16_t temp_avg = 0;
uint8_t ADC_Read_i = 0;


/* ================= MAIN ================= */

void main(void)
{
    /* ---------- Initialization ---------- */

    pins();

    UART_Init(9600);

    ADC_Init();

    Timer0_1ms_Init();

    PWM_Init();

    I2C1_Init();

    display_init();

    display_set_brightness(1);


    /* ---------- Variables ---------- */

    uint16_t srt_temp =
        (uint16_t)((set_temp * 351UL) / 553UL);

    uint16_t oldsrt_temp = srt_temp;

    int32_t dtt = 0;


    /* ================= MAIN LOOP ================= */

    while (1)
    {

        /* ==========================================
           Calculate set temperature
           ========================================== */

        srt_temp =
            (uint16_t)((set_temp * 351UL) / 553UL);


        /* ==========================================
           PID / Temperature processing
           ========================================== */

        if (pid_en)
        {
            uint32_t sum = 0;


            /* ---------- Calculate 50-sample sum ---------- */

            for (uint8_t p = 0; p < AVG_SAMPLES; p++)
            {
                sum += avr_t[p];
            }


            /* ---------- Calculate average ---------- */

            temp_avg =
                (uint16_t)(sum / AVG_SAMPLES);


            /* ---------- Temperature control ---------- */

            tempSit(temp_avg);


            /* PID request handled */

            pid_en = 0;


            /* ---------- Convert ADC average to temperature ---------- */

            uint16_t temp_avg_c =
                (uint16_t)((temp_avg * 351UL) / 553UL);


            /* ---------- Display ---------- */

            if (!SW_Dp)
            {
                /* Display measured temperature */

                display_write_number(temp_avg_c);
            }
            else
            {
                /* Display set temperature */

                display_write_number(srt_temp);
            }
        }


        /* ==========================================
           1 ms timer event
           ========================================== */

        if (tick_1ms)
        {
            tick_1ms = 0;


            /* ======================================
               Read ADC and store sample
               ====================================== */

            avr_t[ADC_Read_i] =
                ADC_Read(ADC_CH_TEMP);


            ADC_Read_i++;


            /* Circular buffer */

            if (ADC_Read_i >= AVG_SAMPLES)
            {
                ADC_Read_i = 0;
            }


            /* ======================================
               PID timing
               ====================================== */

            tick_pid++;


            if (tick_pid >= PID_PERIOD_MS)
            {
                tick_pid = 0;

                pid_en = 1;
            }


            /* ======================================
               Set-temperature display timer
               ====================================== */

            if (SW_Dp)
            {
                tick_1s++;


                if (tick_1s >= SET_DISPLAY_MS)
                {
                    tick_1s = 0;

                    SW_Dp = 0;
                }
            }


            /* ======================================
               Detect set-temperature change
               ====================================== */

            dtt =
                (int32_t)srt_temp -
                (int32_t)oldsrt_temp;


            /* Absolute value */

            if (dtt < 0)
            {
                dtt = -dtt;
            }


            /* ======================================
               Set temperature changed significantly
               ====================================== */

            if (dtt > 20)
            {
                oldsrt_temp = srt_temp;

                SW_Dp = 1;

                tick_1s = 0;
            }
        }
    }
}