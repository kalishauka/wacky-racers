#include "target.h"
#include "adxl345_PWM.h"
#include "radio_module.h"
#include "ledtape_utils.h"
#include "ledbuffer.h"

int main(void)
{
    // TODO: write hat program here...
    usb_serial_stdio_init();
    radio_init();

    radio_payload_t motor_data;

    float left_value = 0;
    float right_value = 0;
    bool reversing = false;

    bool blue = false;
    int count = 0;

    // ledbuffer_t *leds = ledbuffer_init(LEDTAPE_PIO, NUM_LEDS);

    uint8_t leds_seq[NUM_LEDS * 3];

    int i;

    for (i = 0; i < NUM_LEDS; i++)
    {
        // Set full green  GRB order
        leds_seq[i * 3] = 255;
        leds_seq[i * 3 + 1] = 50;
        leds_seq[i * 3 + 2] = 150;
    }

    pacer_init(30);

    while (1)
    {
        if (get_PWM(&left_value, &right_value, &reversing))
        {
            // motor_data.left_motor, motor_data.right_motor, motor_data.reversing
            motor_data.left_motor = left_value;
            motor_data.right_motor = right_value;
            motor_data.reversing = reversing;

            printf("Left Motor: %.2f || Right Motor: %.2f|| Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

            radio_send_data(&motor_data);

            // set_pattern(&count, leds, &blue);

            set_pattern_simple(leds_seq);
        }
    }
}
