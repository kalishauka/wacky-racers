#include "target.h"
#include <stdio.h>
#include "adxl345_PWM.h"
#include "radio_module.h"
#include "ledtape_utils.h"
#include "ledbuffer.h"
#include "battery_level.h"

int main(void)
{
    // TODO: write hat program here...
    usb_serial_stdio_init();
    radio_init();
    adc_t adc = battery_init();

    radio_payload_t motor_data;

    float left_value = 0;
    float right_value = 0;
    bool reversing = false;

    int count = 0;

    // ledbuffer_t *leds = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);

    uint8_t leds_seq[NUM_LEDS * 3];

    pacer_init(30);

    while (1)
    {

        bool battery_good = check_battery_level(adc);
        // count++;

        /*if (count % 1 == 0)
        {

            check_battery_level(adc);
        }*/

        if (get_PWM(&left_value, &right_value, &reversing))
        {
            // motor_data.left_motor, motor_data.right_motor, motor_data.reversing
            motor_data.left_motor = left_value;
            motor_data.right_motor = right_value;
            motor_data.reversing = reversing;

            printf("Left Motor: %.2f || Right Motor: %.2f|| Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

            radio_send_data(&motor_data);

            set_pattern_simple(leds_seq);
        }
    }
}
