#ifndef RADIO_MODULE_H
#define RADIO_MODULE_H

#include "nrf24.h"
#include "pio.h"
#include "pacer.h"
#include "stdio.h"
#include "delay.h"
#include "panic.h"
#include "adxl345_PWM.h"

//#define RADIO_CHANNEL 4
#define RADIO_ADDRESS 0x0123456789LL
#define RADIO_PAYLOAD_SIZE 32

typedef struct
{
    float left_motor;
    float right_motor;
    bool reversing;
} radio_payload_t;

void radio_init(void);

void radio_send_data(radio_payload_t *payload);
//, float right_value, bool reversing
#endif // RADIO_MODULE_H
