#include "target.h"
#include "adxl345_PWM.h" 

int
main (void)
{
    // TODO: write hat program here...
    while (1) {
        float* PWM_values = get_PWM();
        printf("Left Motor: %f || Right Motor: %f || Reversing : %d\n", PWM_values[0], PWM_values[1], PWM_values[2]);
    }

}
