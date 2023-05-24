#ifndef BUZZER_DRIVER_H
#define BUZZER_DRIVER_H
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

#define PWM4_PIO PA2_PIO
#define TUNE_BPM 100
#define DF1 "G/A/GF#E/"
#define BUZZER_SOUND "B5F5/ F5E5D5C5E4E4C4"
#define PWM_FREQ_HZ 100e3
#define BUZZER_UPDATE_RATE 1000



void buzzer_init(void);

void buzzer_beep(void);

void buzzer_update(void);

void bumper_buzzer_hit(void);

#endif