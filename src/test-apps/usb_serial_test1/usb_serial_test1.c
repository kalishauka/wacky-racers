#include <stdio.h>
#include "usb_serial.h"
#include "pio.h"
#include "delay.h"
#include <stdlib.h>

#define HELLO_DELAY_MS 500

int main(void)
{
    int i = 0;

    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_output_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);

    pio_config_set(LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_output_set(LED_ERROR_PIO, !PIO_OUTPUT_HIGH);
    
    // Redirect stdio to USB serial
    usb_serial_stdio_init();

    while (1)
    {
        char channel_id[5];
        sprintf(channel_id, "%d%d%d%d", pio_input_get(RADIO_CH0_PIO), pio_input_get(RADIO_CH1_PIO), pio_input_get(RADIO_CH2_PIO), pio_input_get(RADIO_CH3_PIO));
        int channel = (int) strtol(channel_id, NULL, 2);
        delay_ms(HELLO_DELAY_MS);
        printf("Hello world %d %s\n", channel, channel_id);
        pio_output_toggle(LED_STATUS_PIO);
    }
}
