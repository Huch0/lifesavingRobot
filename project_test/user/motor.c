#include "motor.h"

extern int flag_front_obstacle;
extern int flag_left_obstacle;
extern int flag_right_obstacle;
extern int flag_back_obstacle;

int motor_mode;
unsigned int motor_delay = 1000000;


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

void motor_init(void)
{
    motor_rcc_configure();
    motor_gpio_configure();
    motor_mode = MOTOR_MANUAL;
    stop();
}



void manual_control(void)
{
    uint16_t user_input = bt_get_user_input();
    
    printf("%d\n", user_input);
    switch (user_input)
    {
    case 119: //w
      printf("go forward\n");
        go_forward();
        bt_send_to_user("go forward\n");
        break;
    case 115: //s
        go_backward();
        bt_send_to_user("go backward\n");
        break;
    case 97: //a
        turn_left();
        bt_send_to_user("turn left\n");
        break;
    case 100: //d
        turn_right();
        bt_send_to_user("turn right\n");
        break;
    case 120: //x
        stop();
        bt_send_to_user("stop\n");
        break;
    case 99: //c
        motor_mode = MOTOR_AUTO;
        bt_send_to_user("Auto mode activated\n");
        break;
    default:
        break;
    }
}

int auto_counter = 0;
void auto_control()
{
    uint16_t user = bt_get_user_input();
    //delay_us(10000);
    //USART_SendData(USART1, 97);
    //delay_us(10);
    printf("%d\n", user);
    if (user == 99) {
        auto_counter = 0;
        motor_mode = MOTOR_MANUAL;
        bt_send_to_user("Manual mode activated\n");
        return;
    }
      
    if (user != 122)
    {   
        auto_counter ++;
        if (auto_counter == 1) return;
        auto_counter = 0;
        motor_mode = MOTOR_MANUAL;
        bt_send_to_user("Manual mode activated\n");
        return;
    }
    else if (!flag_front_obstacle)
    {
        go_forward();
        bt_send_to_user("go forward\n");
    }
    else if (!flag_left_obstacle)
    {
        turn_left();
        bt_send_to_user("turn left\n");
    }
    else if (!flag_right_obstacle)
    {
        turn_right();
        bt_send_to_user("turn right\n");
    }
    /*
    else if (!flag_back_obstacle)
    {
        turn_left();
        bt_send_to_user("turn left\n");
        turn_left();
        bt_send_to_user("turn left\n");
        //go_backward();
        //bt_send_to_user("go backward\n");
    }
*/
    else
    {
        go_backward();
        bt_send_to_user("go backward\n");
        //stop();
        //bt_send_to_user("stop\n");
    }
    delay_us(1000000); //1s
    USART_SendData(USART1, user);
    bt_send_to_user((char*)user);
}

void motor_control()
{
    if (motor_mode == MOTOR_MANUAL)
    {
        manual_control();
    }
    else if (motor_mode == MOTOR_AUTO)
    {
        auto_control();
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

    delay_us(300000); // 0.3s
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

    delay_us(300000); // 0.3s
    stop();
}

void turn_left(void)
{
    // Fr ont Left: go backward
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

    delay_us(500000); // 0.5s
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

    delay_us(500000); // 0.5s
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