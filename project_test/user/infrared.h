#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void infrared_init(void);
void infrared_rcc_configure(void);
void infrared_gpio_configure(void);
void infrared_exti_configure(void);

// TODO: Change the port PA9 -> PD10
#define IR_SENSOR_PORT GPIOD
#define IR_SENSOR_PIN GPIO_Pin_10
#define IR_SENSOR_RCC RCC_APB2Periph_GPIOD
#define IR_SENSOR_EXTI_LINE EXTI_Line10
#define IR_SENSOR_EXTI_IRQn EXTI15_10_IRQn

void reset_ir_flag(void);