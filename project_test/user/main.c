#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "motor.h"
#include "ultra_sonic.h"
// #include "bluetooth.h"
#include "infrared.h"

// motor.h
extern int motor_mode;

// ultra_sonic.h
extern ultra_sonic us_front;
extern ultra_sonic us_left;
extern ultra_sonic us_right;
extern ultra_sonic us_back;

// infrared.h
extern volatile uint8_t ir_interrupt_flag;
extern volatile uint32_t ir_timer;

void SysTick_init(void)
{
    // Calculate the reload value for 1ms interval
    uint32_t reload_value = (SystemCoreClock / 1000) - 1;

    // Set the reload value to configure the SysTick timer
    SysTick->LOAD = reload_value;

    // Set the SysTick current value register to 0
    SysTick->VAL = 0;

    // Set the SysTick control and status register:
    // - Enable SysTick
    // - Enable SysTick interrupt
    // - Use system clock as the clock source
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
}

void SysTick_Handler(void)
{
    // Your code to handle SysTick interrupt (e.g., update counters, flags)
    if (ir_interrupt_flag)
    {
        ir_timer++;
    }
}

int main()
{
    SystemInit();
    SysTick_init();

    motor_init();
    ultra_sonic_init();
    // BluetoothInit();
    infrared_init();

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

        reset_ir_flag();
    }

    return 0;
}