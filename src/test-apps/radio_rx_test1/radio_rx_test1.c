/* File:   radio_rx_test1.c
   Author: M. P. Hayes, UCECE
   Date:   24 Feb 2018
*/
#include "nrf24.h"
#include "usb_serial.h"
#include "pio.h"
#include "delay.h"

#define RADIO_CHANNEL 4
#define RADIO_ADDRESS 0x0123456789LL

static void panic (void)
{
    while(1)
    {
        pio_output_toggle (LED1_PIO);
        delay_ms (400);
    }
}

int main(void)
{
    nrf24_cfg_t nrf24_cfg =
        {
            .channel = RADIO_CHANNEL,
            .address = RADIO_ADDRESS,
            .payload_size = 32,
            .ce_pio = RADIO_CE_PIO,
            .irq_pio = RADIO_IRQ_PIO,
            .spi =
            {
                .channel = 0,
                .clock_speed_kHz = 1000,
                .cs = RADIO_CS_PIO,
                .mode = SPI_MODE_0,
                .cs_mode = SPI_CS_MODE_FRAME,
                .bits = 8
            }
        };
    nrf24_t *nrf;

    // Configure LED PIO as output.
    pio_config_set (LED1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (LED2_PIO, PIO_OUTPUT_HIGH);

    // Redirect stdio to USB serial.
    usb_serial_stdio_init ();

#ifdef RADIO_PWR_EN
    // Enable radio regulator if present.
    pio_config_set (RADIO_PWR_EN, PIO_OUTPUT_HIGH);
    delay_ms (10);
#endif

    nrf = nrf24_init (&nrf24_cfg);
    if (! nrf)
        panic ();

    while(1)
    {
        char buffer[33];
        uint8_t bytes;

        bytes = nrf24_read (nrf, buffer, sizeof (buffer));
        if (bytes != 0)
        {
            buffer[bytes] = 0;
            printf ("%s\n", buffer);
            pio_output_toggle (LED1_PIO);
        }
    }
}
