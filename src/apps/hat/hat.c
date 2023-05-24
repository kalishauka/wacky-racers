#include "target.h"
#include <stdio.h>
#include <stdlib.h>
#include "adxl345_PWM.h"
#include "radio_module.h"
#include "ledtape_utils.h"
#include "ledbuffer.h"
#include "battery_level.h"
#include "buzzer_driver.h"
#include "usb_serial.h"
#include "delay.h"
#include "pio.h"

int main(void)
{
    usb_serial_stdio_init();
    radio_init();

    adc_t adc;
    adc = battery_init(adc);
    buzzer_init();
    pacer_init(1000);

    radio_payload_t motor_data;

    float left_value = 0;
    float right_value = 0;
    bool reversing = false;

    uint8_t leds_seq[NUM_LEDS * 3];

    int ticks = 0;
    int count = 0;
    bool bump;
    // buzzer_beep();

    twi_t adxl345_twi;
    adxl345_t *adxl345;

    // Initialise the TWI (I2C) bus for the ADXL345
    adxl345_twi = twi_init(&adxl345_twi_cfg);

    if (!adxl345_twi)
        panic(LED_ERROR_PIO, 1);

    // Initialise the ADXL345
    adxl345 = adxl345_init(adxl345_twi, ADXL345_ADDRESS);

    if (!adxl345)
        panic(LED_ERROR_PIO, 2);

    while (1)
    {
        count++;

        delay_ms(1);
        bool battery_good = check_battery_level(&adc);

        // buzzer_update();

        set_pattern_simple(leds_seq);

        if (count % 15 == 0)
        {

            bump = recieve_radio_data();
        }

        if (bump)
        {
            // play song
        }

        ticks++;
        if (ticks < PACER_RATE / ACCEL_POLL_RATE)
            continue;
        ticks = 0;

        if (get_PWM(&left_value, &right_value, &reversing, adxl345))
        {
            // motor_data.left_motor, motor_data.right_motor, motor_data.reversing
            motor_data.left_motor = left_value;
            motor_data.right_motor = right_value;
            motor_data.reversing = reversing;

            printf("Left Motor: %.2f || Right Motor: %.2f|| Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

            radio_send_data(&motor_data);
        }
    }
}
