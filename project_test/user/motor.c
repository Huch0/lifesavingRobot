#include "motor.h"

extern int flag_front_obstacle;
extern int flag_left_obstacle;
extern int flag_right_obstacle;
extern int flag_back_obstacle;

void motor_init(void)
{
    motor_rcc_configure();
    motor_gpio_configure();
    motor_mode = MOTOR_AUTO;
}

void motor_rcc_configure(void)
{
    RCC_APB2PeriphClockCmd(MOTOR_FL1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_FL2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_FR1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_FR2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_BL1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_BL2_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_BR1_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(MOTOR_BR2_RCC, ENABLE);
}

void motor_gpio_configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = MOTOR_FL1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_FL1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_FL2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_FL2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_FR1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_FR1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_FR2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_FR2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_BL1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_BL1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_BL2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_BL2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_BR1_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_BR1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MOTOR_BR2_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(MOTOR_BR2_PORT, &GPIO_InitStructure);
}

void manual_control(void)
{
    if (motor_mode == MOTOR_AUTO)
    {
        // Raise error
    }
    // TODO
}

void auto_control()
{
    if (!flag_front_obstacle)
    {
        go_forward();
    }
    else if (!flag_left_obstacle)
    {
        turn_left();
    }
    else if (!flag_right_obstacle)
    {
        turn_right();
    }
    else if (!flag_back_obstacle)
    {
        go_backward();
    }
    else
    {
        stop();
    }
}

void go_forward(void)
{
    GPIO_SetBits(MOTOR_FL1_PORT, MOTOR_FL1_PIN);
    GPIO_ResetBits(MOTOR_FL2_PORT, MOTOR_FL2_PIN);

    GPIO_SetBits(MOTOR_FR1_PORT, MOTOR_FR1_PIN);
    GPIO_ResetBits(MOTOR_FR2_PORT, MOTOR_FR2_PIN);

    GPIO_SetBits(MOTOR_BL1_PORT, MOTOR_BL1_PIN);
    GPIO_ResetBits(MOTOR_BL2_PORT, MOTOR_BL2_PIN);

    GPIO_SetBits(MOTOR_BR1_PORT, MOTOR_BR1_PIN);
    GPIO_ResetBits(MOTOR_BR2_PORT, MOTOR_BR2_PIN);

    delay(motor_delay);
    stop();
}

void go_backward(void)
{
    GPIO_ResetBits(MOTOR_FL1_PORT, MOTOR_FL1_PIN);
    GPIO_SetBits(MOTOR_FL2_PORT, MOTOR_FL2_PIN);

    GPIO_ResetBits(MOTOR_FR1_PORT, MOTOR_FR1_PIN);
    GPIO_SetBits(MOTOR_FR2_PORT, MOTOR_FR2_PIN);

    GPIO_ResetBits(MOTOR_BL1_PORT, MOTOR_BL1_PIN);
    GPIO_SetBits(MOTOR_BL2_PORT, MOTOR_BL2_PIN);

    GPIO_ResetBits(MOTOR_BR1_PORT, MOTOR_BR1_PIN);
    GPIO_SetBits(MOTOR_BR2_PORT, MOTOR_BR2_PIN);

    delay(motor_delay);
    stop();
}

void turn_left(void)
{
    // Front Left: go backward
    GPIO_ResetBits(MOTOR_FL1_PORT, MOTOR_FL1_PIN);
    GPIO_SetBits(MOTOR_FL2_PORT, MOTOR_FL2_PIN);

    // Front Right: go forward
    GPIO_SetBits(MOTOR_FR1_PORT, MOTOR_FR1_PIN);
    GPIO_ResetBits(MOTOR_FR2_PORT, MOTOR_FR2_PIN);

    // Back Left: Stop
    GPIO_ResetBits(MOTOR_BL1_PORT, MOTOR_BL1_PIN);
    GPIO_ResetBits(MOTOR_BL2_PORT, MOTOR_BL2_PIN);

    // Back Right: go forward
    GPIO_SetBits(MOTOR_BR1_PORT, MOTOR_BR1_PIN);
    GPIO_ResetBits(MOTOR_BR2_PORT, MOTOR_BR2_PIN);

    delay(motor_delay);
    stop();
}

void turn_right(void)
{
    // Front Left: go forward
    GPIO_SetBits(MOTOR_FL1_PORT, MOTOR_FL1_PIN);
    GPIO_ResetBits(MOTOR_FL2_PORT, MOTOR_FL2_PIN);

    // Front Right: go backward
    GPIO_ResetBits(MOTOR_FR1_PORT, MOTOR_FR1_PIN);
    GPIO_SetBits(MOTOR_FR2_PORT, MOTOR_FR2_PIN);

    // Back Left: go forward
    GPIO_SetBits(MOTOR_BL1_PORT, MOTOR_BL1_PIN);
    GPIO_ResetBits(MOTOR_BL2_PORT, MOTOR_BL2_PIN);

    // Back Right: Stop
    GPIO_ResetBits(MOTOR_BR1_PORT, MOTOR_BR1_PIN);
    GPIO_ResetBits(MOTOR_BR2_PORT, MOTOR_BR2_PIN);

    delay(motor_delay);
    stop();
}

void stop(void)
{
    GPIO_ResetBits(MOTOR_FL1_PORT, MOTOR_FL1_PIN);
    GPIO_ResetBits(MOTOR_FL2_PORT, MOTOR_FL2_PIN);

    GPIO_ResetBits(MOTOR_FR1_PORT, MOTOR_FR1_PIN);
    GPIO_ResetBits(MOTOR_FR2_PORT, MOTOR_FR2_PIN);

    GPIO_ResetBits(MOTOR_BL1_PORT, MOTOR_BL1_PIN);
    GPIO_ResetBits(MOTOR_BL2_PORT, MOTOR_BL2_PIN);

    GPIO_ResetBits(MOTOR_BR1_PORT, MOTOR_BR1_PIN);
    GPIO_ResetBits(MOTOR_BR2_PORT, MOTOR_BR2_PIN);
}