#include <stdio.h>
#include "usb_serial.h"
#include "adc.h"
#include "pacer.h"
#include "panic.h"
#include "battery_level.h"

void check_battery_level(void)
{
    adc_t adc;
    int count = 0;

    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    // Redirect stdio to USB serial
    usb_serial_stdio_init();

    adc = adc_init(&adc_cfg);
    if (!adc)
        panic(LED_ERROR_PIO, 1);

    pacer_init(PACER_RATE);
    while (1)
    {
        float data[10];

        pacer_wait();

        adc_read(adc, data, sizeof(data));
        printf("%3d: %f\n", count++, data);
        pio_output_toggle(LED_STATUS_PIO);
    }
}
