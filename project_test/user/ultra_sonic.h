#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

void ultra_sonic_init(void);
void ultra_sonic_rcc_configure(void);
void ultra_sonic_gpio_configure(void);
void ultra_sonic_timer_configure(void);
void update_obstacle_flags(void);

int flag_front_obstacle = 0;
int flag_left_obstacle = 0;
int flag_right_obstacle = 0;
int flag_back_obstacle = 0;

#define SAFE_DISTANCE 10 // in cm
typedef struct _ultra_sonic
{
    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;
    uint32_t trig_rcc;

    GPIO_TypeDef *echo_port;
    uint16_t echo_pin;
    uint32_t echo_rcc;
    uint32_t distance;
} ultra_sonic;

ultra_sonic us_front = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_0,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_1,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1};

ultra_sonic us_left = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_2,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_3,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1};

ultra_sonic us_right = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_4,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_5,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1};

ultra_sonic us_back = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_6,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_7,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1};