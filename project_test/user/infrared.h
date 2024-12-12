#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void infrared_init(void);
void infrared_rcc_configure(void);
void infrared_gpio_configure(void);
void infrared_exti_configure(void);


#define IR_SENSOR_PORT GPIOA
#define IR_SENSOR_PIN GPIO_Pin_8
#define IR_SENSOR_RCC RCC_APB2Periph_GPIOA
#define IR_SENSOR_EXTI_LINE EXTI_Line8
#define IR_SENSOR_EXTI_IRQn EXTI9_5_IRQn


void reset_ir_flag(void);