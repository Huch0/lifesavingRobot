#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

// #include "sensor.h"
#include "motor.h"
#include "ultra_sonic.h"
// #include "bluetooth.h"

extern int motor_mode;

extern ultra_sonic us_front;
extern ultra_sonic us_left;
extern ultra_sonic us_right;
extern ultra_sonic us_back;

int main()
{
    SystemInit();

    motor_init();
    // SensorInit();
    // CameraInit();
    // BluetoothInit();

    while (1)
    {
        // update the obstacle flags
        update_obstacle_flags();

        if (motor_mode == MOTOR_MANUAL)
        {
            // manual_control(user_input);
        }
        else if (motor_mode == MOTOR_AUTO)
        {
            auto_control();
        }
    }

    return 0;
}