#include "target.h"
#include <stdio.h>
#include <stdlib.h>
#include "adxl345_PWM.h"
#include "radio_module.h"
#include "ledtape_utils.h"
#include "ledbuffer.h"
// #include "battery_level.h"
// #include "buzzer_driver.h"
#include "usb_serial.h"
#include "delay.h"
#include "pio.h"

#define PACER_RATE 1000
#define RADIO_TRANSMIT_RATE 10
#define RADIO_RECIEVE_RATE 100
#define BUZZER_UPDATE_RATE 1000
#define LED_TAPE_UPDATE_RATE 25
#define ACCEL_UPDATE_RATE 1000
#define should_run(task_rate, counter) ((counter) % (PACER_RATE / (task_rate)) == 0)

int main(void)
{
    uint16_t loop_count = 0;
    usb_serial_stdio_init();
    radio_init();
    pacer_init(1000);
    // buzzer_init();

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
        loop_count = (loop_count % PACER_RATE) + 1;

        pacer_wait();

        if (should_run(LED_TAPE_UPDATE_RATE, loop_count))
        {
            set_pattern_simple(leds_seq);
        }
        if (should_run(100, loop_count))
        {
            if (get_PWM(&left_value, &right_value, &reversing, adxl345))
            {
                motor_data.left_motor = left_value;
                motor_data.right_motor = right_value;
                motor_data.reversing = reversing;

                printf("Left Motor: %.2f || Right Motor: %.2f|| Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

                radio_send_data(&motor_data);
            }
        }
    }
}

// if (should_run(RADIO_RECIEVE_RATE, loop_count))
//{
//     bool bumper_hit = receive_radio_data();
//     printf("%d", bumper_hit);
// }

// if (should_run(BUZZER_UPDATE_RATE, loop_count))
//{
//     buzzer_beep();
// }

// ticks++;
// if (ticks < PACER_RATE / ACCEL_POLL_RATE)
//     continue;
// ticks = 0;

// if (get_PWM(&left_value, &right_value, &reversing, adxl345))
// {
//     // motor_data.left_motor, motor_data.right_motor, motor_data.reversing
//     motor_data.left_motor = left_value;
//     motor_data.right_motor = right_value;
//     motor_data.reversing = reversing;

//     // printf("Left Motor: %.2f || Right Motor: %.2f|| Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

//     radio_send_data(&motor_data);
// }
