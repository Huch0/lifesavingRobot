#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"
#include "misc.h"

#include "motor.h"
#include "ultra_sonic.h"
#include "bluetooth.h"
#include "infrared.h"

// motor.h
//extern int motor_mode;

// ultra_sonic.h
extern ultra_sonic us_front;
extern ultra_sonic us_left;
extern ultra_sonic us_right;
extern ultra_sonic us_back;

// infrared.h
extern volatile uint8_t ir_interrupt_flag;
extern volatile uint32_t ir_timer;
/*
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
*/

int main()
{
    SystemInit();
    //SysTick_init();

    motor_init();
    ultra_sonic_init();
    bt_init();
    infrared_init();

    while (1)
    {
        // update the obstacle flags
        update_obstacle_flags();
        printf("us_f: %d | l: %d | r: %d | b: %d\n", us_front.distance, us_left.distance, us_right.distance, us_back.distance);
        

        // control the motor based on the mode
        // motor_control();

        // check the infrared sensor flag
        if (ir_interrupt_flag)
        {
            bt_send_to_user("human detected - check the camera");
            reset_ir_flag(); // reset the flag after 200ms
        }
    }

    return 0;
}