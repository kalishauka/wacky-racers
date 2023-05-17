/* File:   radio_module.c
   Author: M. P. Hayes, UCECE
   Date:   24 Feb 2018
*/
#include "nrf24.h"
#include "pio.h"
#include "pacer.h"
#include "stdio.h"
#include "delay.h"
#include "panic.h"
#include "adxl345_PWM.h"
#include "radio_module.h"

void send_data(float left_value, float right_value, bool reversing)
{
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
            .channel = RADIO_CHANNEL,
            .address = RADIO_ADDRESS,
            .payload_size = RADIO_PAYLOAD_SIZE,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
            .spi = spi_cfg,
        };

    uint8_t count = 0;
    nrf24_t *nrf;

    // Configure LED PIO as output.
    pio_config_set(LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_config_set(LED_STATUS_PIO, PIO_OUTPUT_HIGH);
    pacer_init(10);

#ifdef RADIO_POWER_ENABLE_PIO
    // Enable radio regulator if present.
    pio_config_set(RADIO_POWER_ENABLE_PIO, PIO_OUTPUT_HIGH);
    delay_ms(10);
#endif

    nrf = nrf24_init(&nrf24_cfg);
    if (!nrf)
        panic(LED_ERROR_PIO, 2);

    while (1)
    {
        char buffer[RADIO_PAYLOAD_SIZE + 1];

        pacer_wait();
        pio_output_toggle(LED_STATUS_PIO);

        // print struct to buffer

        snprintf(buffer, sizeof(buffer), "%.2f %.2f %d", left_value, right_value, reversing);

        if (!nrf24_write(nrf, buffer, RADIO_PAYLOAD_SIZE))
            pio_output_set(LED_ERROR_PIO, 0);
        else
            pio_output_set(LED_ERROR_PIO, 1);
    }
}
