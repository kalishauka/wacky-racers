#include "target.h"
#include "adxl345_PWM.h"
#include "radio_module.h"

int main(void)
{
    // TODO: write hat program here...
    usb_serial_stdio_init();

    while (1)
    {
        float left_value = 0;
        float right_value = 0;
        bool reversing = false;

        bool res = get_PWM(&left_value, &right_value, &reversing);
        if (res)
        {
            printf("Left Motor: %f || Right Motor: %f || Reversing : %d struct\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);

            send_data(left_value, right_value, reversing);
        }
    }
}
