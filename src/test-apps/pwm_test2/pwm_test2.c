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


#define PWM4_PIO PA2_PIO

#define PWM_FREQ_HZ 3000


static const pwm_cfg_t pwm4_cfg =
    {
        .pio = PWM4_PIO,
        .period = PWM_PERIOD_DIVISOR(PWM_FREQ_HZ),
        .duty = PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 0),
        .align = PWM_ALIGN_LEFT,
        .polarity = PWM_POLARITY_HIGH,
        .stop_state = PIO_OUTPUT_LOW};

int main(void)
{
    usb_serial_stdio_init();

    pwm_t pwm4;

    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);


    pwm4 = pwm_init(&pwm4_cfg);
    if (!pwm4)
        panic(LED_ERROR_PIO, 2);

    pwm_channels_start(pwm_channel_mask(pwm4));

    while (1)
    {
        
       
        pwm_duty_ppt_set(pwm4, 500);
            
        

        delay_ms(500);
        pio_output_toggle(LED_STATUS_PIO);
    }

    return 0;
}