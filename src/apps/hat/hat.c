#include "target.h"
#include "adxl345_PWM.h"

int main(void)
{
    // TODO: write hat program here...
    usb_serial_stdio_init();

    struct motor_values_t motor_data;

    while (1)
    {
        float left_value = 0;
        float right_value = 0;
        bool reversing = false;

        bool res = get_PWM(&left_value, &right_value, &reversing);
        if (res)
        {
            printf("Left Motor: %f || Right Motor: %f || Reversing : %d poto\n", left_value, right_value, reversing);

            motor_data.right_motor = right_value;
            motor_data.left_motor = left_value;
            motor_data.reversing = reversing;
            // printf("Left Motor: %f || Right Motor: %f || Reversing : %d poto\n", motor_data.left_motor, motor_data.right_motor, motor_data.reversing);
        }
    }
}
