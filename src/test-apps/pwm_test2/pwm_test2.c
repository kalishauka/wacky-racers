/* File:   pwm_test2.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr:  This example starts two channels simultaneous
   ly; one inverted
           with respect to the other.
*/
#include "pwm.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"
#include <stdio.h>
#include "usb_serial.h"
#include <string.h>

#define PWM1_PIO PA1_PIO
#define PWM2_PIO PA7_PIO
#define PWM3_PIO PA0_PIO
#define PWM4_PIO PA2_PIO

#define PWM_FREQ_HZ 100e3


static const pwm_cfg_t pwm1_cfg =
{
    .pio = PWM1_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm2_cfg =
{
    .pio = PWM2_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 50),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm3_cfg =
{
    .pio = PWM3_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 0),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

static const pwm_cfg_t pwm4_cfg =
{
    .pio = PWM4_PIO,
    .period = PWM_PERIOD_DIVISOR (PWM_FREQ_HZ),
    .duty = PWM_DUTY_DIVISOR (PWM_FREQ_HZ, 0),
    .align = PWM_ALIGN_LEFT,
    .polarity = PWM_POLARITY_HIGH,
    .stop_state = PIO_OUTPUT_LOW
};

int
main (void)
{
    usb_serial_stdio_init ();

    pwm_t pwm1;
    pwm_t pwm2;
    pwm_t pwm3;
    pwm_t pwm4;

    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    pwm1 = pwm_init (&pwm1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    pwm2 = pwm_init (&pwm2_cfg);
    if (! pwm2)
        panic (LED_ERROR_PIO, 2);
           
    pwm3 = pwm_init (&pwm3_cfg);
    if (! pwm3)
        panic (LED_ERROR_PIO, 2);

    pwm4 = pwm_init (&pwm4_cfg);
    if (! pwm4)
        panic (LED_ERROR_PIO, 2);

    pwm_channels_start (pwm_channel_mask (pwm1) | pwm_channel_mask (pwm2) | pwm_channel_mask (pwm3) | pwm_channel_mask (pwm4));

    while (1)
    {
        int duty_cycle1;
        int duty_cycle2;
        int direction1;
        int direction2;
        //If direction = 1, go forward, if = 0, go back
        // Read user input
        char buf[256];
        if (fgets(buf, sizeof(buf), stdin)) {
            // sscanf returns the number of input items successfully matched
            if (sscanf(buf, "%d %d %d %d",&duty_cycle1, &direction1, &duty_cycle2, &direction2) == 4) {
                printf("%d, %d, %d, %d\n", duty_cycle1, direction1, duty_cycle2, direction2);
            } else {
                printf("Invalid input\n");
            }
        }


        switch (direction1) {
            case 0:
                pwm_duty_ppt_set(pwm1, duty_cycle1*10);
                pwm_duty_ppt_set(pwm3, 0);
                break;
            case 1:
                pwm_duty_ppt_set(pwm3, duty_cycle1*10);
                pwm_duty_ppt_set(pwm1, 0);
                break;
        }

        switch (direction2) {
            case 0:
                pwm_duty_ppt_set(pwm2, duty_cycle2*10);
                pwm_duty_ppt_set(pwm4, 0);
                break;
            case 1:
                pwm_duty_ppt_set(pwm4, duty_cycle2*10);
                pwm_duty_ppt_set(pwm2, 0);
                break;
        }

        delay_ms (500);
        pio_output_toggle (LED_STATUS_PIO);
    }

    return 0;
}
