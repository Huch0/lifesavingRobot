#include "bluetooth.h"

void bt_init(void)
{
    bt_rcc_configure();
    bt_gpio_configure();
    bt_usart2_configure();
    bt_nvic_configure();
}

void bt_rcc_configure(void)
{
    /* USART2 TX/RX port clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    /* USART2 clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Alternate Function IO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void bt_gpio_configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* USART2 pin setting */
    // TX (PA2)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // RX (PA3)
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void bt_usart2_configure(void)
{
    USART_InitTypeDef USART2_InitStructure;

    // Enable the USART2 peripheral
    USART_Cmd(USART2, ENABLE);

    // Initialize the USART using the structure 'USART_InitTypeDef' and the function 'USART_Init'
    USART2_InitStructure.USART_BaudRate = 9600;
    USART2_InitStructure.USART_StopBits = USART_StopBits_1;
    USART2_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART2_InitStructure.USART_Parity = USART_Parity_No;
    USART2_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART2_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART2_InitStructure);

    // Enable the USART2 RX interrupts using the function 'USART_ITConfig' and the argument value 'Receive Data register not empty interrupt'
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void bt_nvic_configure(void)
{

    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // USART2
    // 'NVIC_EnableIRQ' is only required for USART setting
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

char user_input = 0;

void USART2_IRQHandler()
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        // Store the most recent received data
        char received = (char)USART_ReceiveData(USART2);

        // Validate the received input
        // 'w' - forward | 'a' - left | 's' - backward | 'd' - right | 'x' - stop | 'c' - change mode
        if (received == 'w' || received == 'a' || received == 's' || received == 'd' || received == 'x' || received == 'c')
        {
            user_input = received; // Valid input
        }
        else
        {
            // Invalid input, send error message
            char error_message[] = "invalid input\n";
            bt_send_to_user(error_message);
        }

        // Clear the 'Read data register not empty' flag
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

char bt_get_user_input(void)
{
    return user_input;
}

void bt_send_to_user(char *message)
{
    for (int i = 0; message[i] != '\0'; i++)
    {
        while ((USART2->SR & USART_SR_TXE) == 0) // Wait until TX buffer is empty
            ;

        USART_SendData(USART2, message[i]);
    }
}