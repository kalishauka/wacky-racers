#include <stdio.h>
#include "usb_serial.h"
#include "adc.h"
#include "pacer.h"
#include "panic.h"
#include "battery_level.h"

adc_t battery_init(void)
{

    adc_t adc;

    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    // Redirect stdio to USB serial
    usb_serial_stdio_init();

    adc = adc_init(&adc_cfg);
    if (!adc)
        panic(LED_ERROR_PIO, 1);

    pacer_init(2);

    return adc;
}

void check_battery_level(adc_t adc)
{
    int count = 0;
    float data[1];

    pacer_wait();

    adc_read(adc, data, sizeof(data));

    if (data[0] < VOLTAGE_THRESHOLD)
    {
        pio_output_high(LED_STATUS_PIO);
    }
    else
    {
        pio_output_low(LED_STATUS_PIO);
    }

    printf("%3d: %f\n", count++, data[0]);
}
