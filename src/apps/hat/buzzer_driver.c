/* File:   pwm_test2.c
   Author: M. P. Hayes, UCECE
   Date:   15 April 2013
   Descr:  This example starts two channels simultaneous
   ly; one inverted
           with respect to the other.
*/
#include "buzzer_driver.h"
#include "pwm.h"
#include "pio.h"
#include "delay.h"
#include "panic.h"
#include <stdio.h>
#include "usb_serial.h"
#include <string.h>
#include "piezo.h"
#include "piezo_beep.h"
#include "mmelody.h"
#include <stdlib.h>
#include "math.h"

#define BUZZER_PIO PA2_PIO
#define TUNE_BPM 100
#define DF1 "G/A/GF#E/"
#define BUZZER_SOUND "B5F5/ F5E5D5C5E4E4C4"
#define PWM_FREQ_HZ 100e3
#define BUZZER_UPDATE_RATE 1000

static const piezo_cfg_t piezo_config = {
    .pio = BUZZER_PIO};

static const pwm_cfg_t pwm4_cfg =
    {
        .pio = BUZZER_PIO,
        .period = PWM_PERIOD_DIVISOR(PWM_FREQ_HZ),
        .duty = PWM_DUTY_DIVISOR(PWM_FREQ_HZ, 50),
        .align = PWM_ALIGN_LEFT,
        .polarity = PWM_POLARITY_HIGH,
        .stop_state = PIO_OUTPUT_LOW};

static piezo_t piezo;

static mmelody_obj_t mmelody_dev;

static mmelody_t mmelody;

static pwm_t pwm4;

static const char death_melody[] = DF1 ":";
static const char another_melody[] = BUZZER_SOUND ":";

void callback(void *data, uint8_t note, uint8_t volume)
{
    if (note < 60)

        pwm_stop(pwm4);

    else
    {
        pwm_start(pwm4);
        uint32_t frequency = 440 * pow(2.0, (note - 69.0) / 12.0);
        pwm_frequency_set(pwm4, frequency);
    }
}

void buzzer_init(void)
{
    piezo = piezo_init(&piezo_config);
    mmelody = mmelody_init(&mmelody_dev, BUZZER_UPDATE_RATE, callback, NULL);
    mmelody_speed_set(mmelody, 225);
    mmelody_play(mmelody, another_melody);

    pwm4 = pwm_init(&pwm4_cfg);
    if (!pwm4)
    {
        panic(LED_ERROR_PIO, 10);
    }

    pwm_start(pwm4);
}

void buzzer_beep(void)
{
    
    piezo_beep_long(piezo);
}

void buzzer_update(void)
{
    mmelody_update(mmelody);
}

void bumper_buzzer_hit(void)
{
    mmelody_play(mmelody, another_melody);
}