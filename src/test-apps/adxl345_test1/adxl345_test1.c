/* File:   radio_data.c
   Author: P. K. Stenger
   Date:   6 May 2023
   Descr:  Read from an ADXL345 accelerometer, transforms into motor PWM's and write its output to the USB serial.
*/

#include "pio.h"
#include "delay.h"
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "adxl345.h"
#include "panic.h"
#include <math.h>

/*
 * NOTE: you must define ADXL345_ADDRESS in target.h for this to compile.
 */

// #define ADXL345_ADDRESS 0x53
#ifndef ADXL345_ADDRESS
#error ADXL345_ADDRESS must be defined
#endif

#define PACER_RATE 20
#define ACCEL_POLL_RATE 1

static twi_cfg_t adxl345_twi_cfg =
{
    .channel = TWI_CHANNEL_0,
    .period = TWI_PERIOD_DIVISOR (100000), // 100 kHz
    .slave_addr = 0
};

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

float find_speed_constant(float y, float default_Speed, float default_reverse_speed, bool *reversing) {
    float speed_constant = 0.0;
    float a = 0.0, c = 0.0, d = 0.0;

    if (y < 0.5) {
        a = default_Speed / 0.25;
        speed_constant = a * pow(y, 2);
        *reversing = false;
    } else {
        c = -(default_Speed - 1) / 0.5;
        d = 1 - c;
        speed_constant = c * y + d;
        *reversing = false;
    }

    if (speed_constant < 0.1) {
        speed_constant = default_reverse_speed;
        *reversing = true;
    }

    return speed_constant;
}

void find_motor_ratio(float x, float speed_constant, float *left_motor, float *right_motor) {
    *left_motor = 1.0;
    *right_motor = 1.0;

    if (x > 0) {
        *right_motor = 1.0 - x;
    } else {
        *left_motor = 1.0 + x;
    }
}

void find_motor_PWM(float speed_constant, float left_motor, float right_motor, float *PWM_left_motor, float *PWM_right_motor) {
    *PWM_right_motor = speed_constant * right_motor;
    *PWM_left_motor = speed_constant * left_motor;
}

int main (void)
{
    twi_t adxl345_twi;
    adxl345_t *adxl345;
    int ticks = 0;
    int count = 0;

    float default_speed = 0.7;
    float default_reverse_speed = 0.8;

    float x, y;
    float speed_constant;
    float PWM_value;
    float left_motor, right_motor;
    float PWM_left_motor, PWM_right_motor;

    bool reversing = false;

    // Redirect stdio to USB serial
    usb_serial_stdio_init ();

    pio_config_set (LED_ERROR_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_ERROR_PIO, ! PIO_OUTPUT_HIGH);
    pio_config_set (LED_STATUS_PIO, PIO_OUTPUT_LOW);
    pio_output_set (LED_STATUS_PIO, ! PIO_OUTPUT_HIGH);

    // Initialise the TWI (I2C) bus for the ADXL345
    adxl345_twi = twi_init (&adxl345_twi_cfg);

    if (! adxl345_twi)
        panic (LED_ERROR_PIO, 1);

    // Initialise the ADXL345
    adxl345 = adxl345_init (adxl345_twi, ADXL345_ADDRESS);

    if (! adxl345)
        panic (LED_ERROR_PIO, 2);

    pacer_init (PACER_RATE);

    while (1)
    {
        /* Wait until next clock tick.  */
        pacer_wait ();

        ticks++;
        if (ticks < PACER_RATE / ACCEL_POLL_RATE)
            continue;
        ticks = 0;

        pio_output_toggle (LED_STATUS_PIO);

        /* Read in the accelerometer data.  */
        if (! adxl345_is_ready (adxl345))
        {
            count++;
            printf ("Waiting for accelerometer to be ready... %d\n", count);
        }
        else
        {
            int16_t accel[3];
            if (adxl345_accel_read (adxl345, accel))
            {

                if (accel[0] > 160) {
                    accel[0] = 160;
                } else if (accel[0] < -160) {
                    accel[0] = -160;
                }

                if (accel[1] > 160) {
                    accel[1] = 160;
                } else if (accel[1] < -160) {
                    accel[1] = -160;
                }

                /* Standardize accelerometer values for easy transformation into PWM*/
                x = map(accel[0], -160, 160, -1, 1);
                y = map(accel[1], -160, 160, 0, 1);

                /* Find speed constant from y value of accelerometer*/
                speed_constant = find_speed_constant(y, default_speed, default_reverse_speed, &reversing);

                /* Find motor ratio from x value of accelerometer*/
                find_motor_ratio(x, speed_constant, &left_motor, &right_motor);

                /* Find PWM values for each motor*/
                find_motor_PWM(speed_constant, left_motor, right_motor, &PWM_left_motor, &PWM_right_motor);

                /* print PWM values to serial monitor*/
                printf("Left Motor: %f || Right Motor: %f || Reversing : %d\n", PWM_left_motor, PWM_right_motor, reversing);

            }
            else
            {
                printf ("ERROR: failed to read acceleration\n");
            }
        }
    }
}