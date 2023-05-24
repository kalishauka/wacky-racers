#include <stdio.h>
#include <stdlib.h>
#include "usb_serial.h"
#include "adc.h"
#include "pacer.h"
#include "panic.h"
#include "battery_level.h"

adc_t battery_init(adc_t adc)
{

    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    adc = adc_init(&adc_cfg);
    if (!adc)
        panic(LED_ERROR_PIO, 1);

    return adc;
}

bool check_battery_level(adc_t *adc)
{
    uint16_t data[1];
    bool result;

    adc_read(*adc, data, sizeof(data));

    if (data[0] < VOLTAGE_THRESHOLD)
    {
        pio_output_high(LED_STATUS_PIO);
        result = 0;
    }
    else
    {
        pio_output_low(LED_STATUS_PIO);
        result = 1;
    }

    // printf("%d\n", data[0]);

    return result;
}
