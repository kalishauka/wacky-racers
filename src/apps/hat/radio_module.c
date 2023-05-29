/* File:   radio_module.c
   Author: M. P. Hayes, UCECE
   Date:   24 Feb 2018
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "nrf24.h"
#include "pio.h"
#include "pacer.h"
#include "stdio.h"
#include "delay.h"
#include "panic.h"
#include "adxl345_PWM.h"
#include "radio_module.h"

void radio_init(void)

{
    char channel_id[5];
    sprintf(channel_id, "%d%d%d%d", pio_input_get(RADIO_CH0_PIO), pio_input_get(RADIO_CH1_PIO), pio_input_get(RADIO_CH2_PIO), pio_input_get(RADIO_CH3_PIO));
    int channel = (int)strtol(channel_id, NULL, 2);
    if (channel == 0)
    {
        channel = 1;
    }

    spi_cfg_t spi_cfg =
        {
            .channel = 0,
            .clock_speed_kHz = 1000,
            .cs = RADIO_CS_PIO,
            .mode = SPI_MODE_0,
            .cs_mode = SPI_CS_MODE_FRAME,
            .bits = 8};
    nrf24_cfg_t nrf24_cfg =
        {
            .channel = channel,
            .address = RADIO_ADDRESS,
            .payload_size = RADIO_PAYLOAD_SIZE,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
            .spi = spi_cfg,
        };

    // Configure LED PIO as output.
    pio_config_set(LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pacer_init(10);

#ifdef RADIO_POWER_ENABLE_PIO
    // Enable radio regulator if present.
    pio_config_set(RADIO_POWER_ENABLE_PIO, PIO_OUTPUT_HIGH);
    delay_ms(10);
#endif

    nrf_handle = nrf24_init(&nrf24_cfg);
    if (!nrf_handle)
    {
        panic(LED_ERROR_PIO, 2);
    }
}

bool receive_radio_data(void)
{
    bool bumped = false;

    uint8_t bytes_read;
    bytes_read = nrf24_read(nrf_handle, &bumped, 1);
    printf("%d", bytes_read);

    return bumped;
}

void radio_send_data(radio_payload_t *payload)
{
    char buffer[RADIO_PAYLOAD_SIZE + 1];

    uint8_t status = nrf24_write(nrf_handle, payload, sizeof(radio_payload_t));
    if (!nrf24_write(nrf_handle, payload, sizeof(radio_payload_t)))

    {
        pio_output_set(LED_ERROR_PIO, 0);
    }
    else
    {
        pio_output_set(LED_ERROR_PIO, 1);
    }
}
