#ifndef ADXL345_PWM_H
#define ADXL345_PWM_H

#include "pio.h"
#include "delay.h"
#include "target.h"
#include "pacer.h"
#include "usb_serial.h"
#include "adxl345.h"
#include "panic.h"
#include <math.h>

// #define PACER_RATE 20
#define ACCEL_POLL_RATE 1
#ifndef LED_ACTIVE
#define LED_ACTIVE PIO_OUTPUT_LOW
#endif

static twi_cfg_t adxl345_twi_cfg =
    {
        .channel = TWI_CHANNEL_0,
        .period = TWI_PERIOD_DIVISOR(100000), // 100 kHz
        .slave_addr = 0};

float map(float value, float fromLow, float fromHigh, float toLow, float toHigh);
float find_speed_constant(float y, float default_Speed, float default_reverse_speed, bool *reversing);
void find_motor_ratio(float x, float speed_constant, float *left_motor, float *right_motor);
void find_motor_PWM(float speed_constant, float left_motor, float right_motor, float *PWM_left_motor, float *PWM_right_motor);
int clamp(int value, int min, int max);
bool get_PWM(float *left_value, float *right_value, bool *reversing, adxl345_t *adxl345);
#endif // ADXL345_PWM_H
