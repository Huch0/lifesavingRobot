#include "ultra_sonic.h"

#define US_TIMEOUT 30000 // Timeout for echo signal in microseconds

#define US_TIMER_f TIM2
#define US_TIMER_f_RCC RCC_APB1Periph_TIM2

#define US_TIMER_l TIM3
#define US_TIMER_l_RCC RCC_APB1Periph_TIM3

#define US_TIMER_r TIM4
#define US_TIMER_r_RCC RCC_APB1Periph_TIM4

#define US_TIMER_b TIM5
#define US_TIMER_b_RCC RCC_APB1Periph_TIM5


ultra_sonic us_front = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_0,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_1,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1,
    .timer = US_TIMER_f
};

ultra_sonic us_left = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_2,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_8,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1,
    .timer = US_TIMER_l
};

ultra_sonic us_right = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_4,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_5,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1,
    .timer = US_TIMER_r
};

ultra_sonic us_back = {
    .trig_port = GPIOA,
    .trig_pin = GPIO_Pin_6,
    .trig_rcc = RCC_APB2Periph_GPIOA,
    .echo_port = GPIOA,
    .echo_pin = GPIO_Pin_7,
    .echo_rcc = RCC_APB2Periph_GPIOA,
    .distance = SAFE_DISTANCE + 1,
    .timer = US_TIMER_b
};

int flag_front_obstacle = 0;
int flag_left_obstacle = 0;
int flag_right_obstacle = 0;
int flag_back_obstacle = 0;

void ultra_sonic_init(void)
{
    ultra_sonic_rcc_configure();
    ultra_sonic_gpio_configure();
    ultra_sonic_timer_configure();
}

void ultra_sonic_rcc_configure(void)
{
    RCC_APB2PeriphClockCmd(us_front.trig_rcc, ENABLE);
    RCC_APB2PeriphClockCmd(us_front.echo_rcc, ENABLE);

    RCC_APB2PeriphClockCmd(us_left.trig_rcc, ENABLE);
    RCC_APB2PeriphClockCmd(us_left.echo_rcc, ENABLE);

    RCC_APB2PeriphClockCmd(us_right.trig_rcc, ENABLE);
    RCC_APB2PeriphClockCmd(us_right.echo_rcc, ENABLE);

    RCC_APB2PeriphClockCmd(us_back.trig_rcc, ENABLE);
    RCC_APB2PeriphClockCmd(us_back.echo_rcc, ENABLE);

    RCC_APB1PeriphClockCmd(US_TIMER_f_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(US_TIMER_l_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(US_TIMER_r_RCC, ENABLE);
    RCC_APB1PeriphClockCmd(US_TIMER_b_RCC, ENABLE);
}

void ultra_sonic_gpio_configure(void)
{
    us_gpio_config(us_front);
    us_gpio_config(us_left);
    us_gpio_config(us_right);
    us_gpio_config(us_back);
}

void us_gpio_config(ultra_sonic us)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = us.trig_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(us.trig_port, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = us.echo_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(us.echo_port, &GPIO_InitStructure);
}

void ultra_sonic_timer_configure(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;

    TIM_BaseStruct.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // 1 µs resolution
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 0xFFFF;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(US_TIMER_f, &TIM_BaseStruct);

    TIM_Cmd(US_TIMER_f, ENABLE);
    

    TIM_BaseStruct.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // 1 µs resolution
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 0xFFFF;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(US_TIMER_l, &TIM_BaseStruct);

    TIM_Cmd(US_TIMER_l, ENABLE);
    

    TIM_BaseStruct.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // 1 µs resolution
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 0xFFFF;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(US_TIMER_r, &TIM_BaseStruct);

    TIM_Cmd(US_TIMER_r, ENABLE);
    

    TIM_BaseStruct.TIM_Prescaler = (SystemCoreClock / 1000000) - 1; // 1 µs resolution
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 0xFFFF;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(US_TIMER_b, &TIM_BaseStruct);

    TIM_Cmd(US_TIMER_b, ENABLE);
}

void update_obstacle_flags(void)
{
    flag_front_obstacle = measure_distance(&us_front) < SAFE_DISTANCE;
    flag_left_obstacle = measure_distance(&us_left) < SAFE_DISTANCE;
    flag_right_obstacle = measure_distance(&us_right) < SAFE_DISTANCE;
    flag_back_obstacle = measure_distance(&us_back) < SAFE_DISTANCE;
}

uint32_t measure_distance(ultra_sonic *us)
{
    uint32_t start_time, pulse_duration;

    // Trigger the ultrasonic sensor
    GPIO_SetBits(us->trig_port, us->trig_pin);
    delay_us(10); // 10 µs trigger pulse
    GPIO_ResetBits(us->trig_port, us->trig_pin);

    // Wait for the echo pin to go high
    start_time = TIM_GetCounter(us->timer);
    //printf("start_time1: %d\n", start_time);
    while (!GPIO_ReadInputDataBit(us->echo_port, us->echo_pin))
    {
        if (((TIM_GetCounter(us->timer) - start_time) & 0xFFFF) > US_TIMEOUT)
        {
            return SAFE_DISTANCE + 1; // Timeout condition
        }
    }

    // Measure the high pulse duration
    start_time = TIM_GetCounter(us->timer);
    while (GPIO_ReadInputDataBit(us->echo_port, us->echo_pin))
    {
        if (((TIM_GetCounter(us->timer) - start_time) & 0xFFFF) > US_TIMEOUT)
        {
            return SAFE_DISTANCE + 1; // Timeout condition
        }
    }
    pulse_duration = ((TIM_GetCounter(us->timer) - start_time) & 0xFFFF);

    // Calculate and return distance (in cm)
    us->distance = (pulse_duration * 34300) / (2 * 1000000);
    return us->distance;
}
