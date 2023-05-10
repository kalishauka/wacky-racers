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



int
main (void)
{
    usb_serial_stdio_init ();
    pio_config_set (PWM3_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PWM4_PIO, PIO_OUTPUT_LOW);
    pwm_t pwm1;
    pwm_t pwm2;

    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    pwm1 = pwm_init (&pwm1_cfg);
    if (! pwm1)
        panic (LED_ERROR_PIO, 1);

    pwm2 = pwm_init (&pwm2_cfg);
    if (! pwm2)
        panic (LED_ERROR_PIO, 2);
           


    pwm_channels_start (pwm_channel_mask (pwm1) | pwm_channel_mask (pwm2));

    while (1)
    {
        int duty_cycle;
        // Read user input
        char buf[256];
        if (fgets(buf, sizeof(buf), stdin)) {
            // sscanf returns the number of input items successfully matched
            if (sscanf(buf, "%d",&duty_cycle) == 1) {
                printf("%d\n", duty_cycle);
            } else {
                printf("Invalid input\n");
            }
        }

        // Limit duty cycle to 0-100
        if (duty_cycle < 0)
            duty_cycle = 0;
        else if (duty_cycle > 100)
            duty_cycle = 100;

        // Update PWM duty cycle
        pwm_duty_set (pwm1, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, duty_cycle));
        pwm_duty_set (pwm2, PWM_DUTY_DIVISOR (PWM_FREQ_HZ, duty_cycle));

        delay_ms (500);
        pio_output_toggle (LED_STATUS_PIO);
    }

    return 0;
}
