#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_exti.h"
#include "misc.h"

void bt_init(void);
void bt_rcc_configure(void);
void bt_gpio_configure(void);
void bt_usart1_configure(void);
void bt_usart2_configure(void);
void bt_nvic_configure(void);

uint16_t bt_get_user_input(void);
void bt_send_to_user(char *message);