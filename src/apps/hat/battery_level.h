#ifndef BATTERY_LEVEL_H
#define BATTERY_LEVEL_H

#include <stdio.h>
#include "usb_serial.h"
#include "adc.h"
#include "pacer.h"
#include "panic.h"

#define PACER_RATE 2

static const adc_cfg_t adc_cfg = {
    .bits = 12,
    .channels = BIT(ADC_CHANNEL_0),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000};

void check_battery_level(void);

#endif /* BATTERY_LEVEL_H */
