#ifndef _ULTRA_
#define _ULTRA_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"


typedef struct _ultra_sonic
{
    GPIO_TypeDef *trig_port;
    uint16_t trig_pin;
    uint32_t trig_rcc;

    GPIO_TypeDef *echo_port;
    uint16_t echo_pin;
    uint32_t echo_rcc;
    uint32_t distance;
    TIM_TypeDef *timer;
} ultra_sonic;

void ultra_sonic_init(void);
void ultra_sonic_rcc_configure(void);
void ultra_sonic_gpio_configure(void);
void us_gpio_config(ultra_sonic us);
void ultra_sonic_timer_configure(void);
void update_obstacle_flags(void);

uint32_t measure_distance(ultra_sonic *us);


#define SAFE_DISTANCE 10 // in cm
#endif

