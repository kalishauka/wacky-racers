#include "target.h"
#include "adxl345_PWM.h"
#include "radio_module.h"

int main(void)
{
    // TODO: write hat program here...
    usb_serial_stdio_init();
    radio_init();

    while (1)
    {
        float left_value = 0;
        float right_value = 0;
        bool reversing = false;

        bool res = get_PWM(&left_value, &right_value, &reversing);

        if (res)
        {

            // motor_data.left_motor, motor_data.right_motor, motor_data.reversing
            radio_payload_t motor_data = {left_value, right_value, reversing};

            printf("Left Motor: %.2f || Right Motor: %.2f|| Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

            // radio_send_data(&motor_data);
            //, right_value, reversing
        }
    }
}
