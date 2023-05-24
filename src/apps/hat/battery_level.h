#ifndef BATTERY_LEVEL_H
#define BATTERY_LEVEL_H

#include <stdio.h>
#include "usb_serial.h"
#include "adc.h"
#include "pacer.h"
#include "panic.h"

#define VOLTAGE_THRESHOLD 2665

adc_t battery_init(adc_t adc);

static const adc_cfg_t adc_cfg = {
    .bits = 12,
    .channels = BIT(ADC_CHANNEL_0),
    .trigger = ADC_TRIGGER_SW,
    .clock_speed_kHz = 1000};

bool check_battery_level(adc_t *adc);

#endif /* BATTERY_LEVEL_H */
