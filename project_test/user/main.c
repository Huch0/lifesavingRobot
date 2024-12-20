#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

#include "motor.h"
#include "ultra_sonic.h"
#include "bluetooth.h"
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

// bluetooth
extern uint16_t is_connected;

int main()
{
    SystemInit();

    motor_init();
    ultra_sonic_init();
    bt_init();
    infrared_init();
    USART_SendData(USART1, 97);
    printf("init\n");
    bt_send_to_user("init\n");
    
    //is_connected = 1;
    while (1)
    {
      // USART_SendData(USART1, 97);
      if (is_connected) {
          //USART_SendData(USART1, 98);
          // update the obstacle flags
        if (motor_mode == MOTOR_AUTO) {
            update_obstacle_flags();
            printf("us_f: %d | l: %d | r: %d | b: %d\n", us_front.distance, us_left.distance, us_right.distance, us_back.distance);
        }
          // control the motor based on the mode
          motor_control();
          // printf("main loop\n");
          //bt_send_to_user("human detected - check the camera\n");
          // check the infrared sensor flag
          if (ir_interrupt_flag)
          {
              printf("human detected - check the camera\n");
              bt_send_to_user("human detected - check the camera\n");
              reset_ir_flag(); // reset the flag after 200ms
          }
      }
    }

    return 0;
}