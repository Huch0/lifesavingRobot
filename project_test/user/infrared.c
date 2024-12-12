#include "infrared.h"

volatile uint8_t ir_interrupt_flag = 0;
volatile uint32_t ir_timer = 0;

void infrared_init(void)
{
    infrared_rcc_configure();
    infrared_gpio_configure();
    infrared_exti_configure();
}

void infrared_rcc_configure(void)
{
    RCC_APB2PeriphClockCmd(IR_SENSOR_RCC, ENABLE);
}

void infrared_gpio_configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = IR_SENSOR_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(IR_SENSOR_PORT, &GPIO_InitStructure);
}

void infrared_exti_configure(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // Enable the EXTI9_5 interrupt
    NVIC_InitStructure.NVIC_IRQChannel = IR_SENSOR_EXTI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // Configure EXTI line
    EXTI_InitStructure.EXTI_Line = IR_SENSOR_EXTI_LINE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/*
 * When the sensor detects a human, the sensor sends a HIGH signal through OUT pin
 * and retains it for 10 seconds.
 * The sensor sends a LOW signal when it detects no human.
 * The interrupt is triggered when the sensor sends a HIGH signal.
 * The interrupt handler sends a message to the user to check the camera.
 */

void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(IR_SENSOR_EXTI_LINE) != RESET)
    {
        EXTI_ClearITPendingBit(IR_SENSOR_EXTI_LINE);

        if (!ir_interrupt_flag) // Check if the flag is already set
        {
            ir_interrupt_flag = 1;

            // Simulate debouncing with a delay or software timer
            // Reset the flag after 200ms (implement this in main loop)
        }
    }
}

void reset_ir_flag(void)
{
    if (ir_interrupt_flag && ir_timer > 200)
    {
        ir_interrupt_flag = 0;
        ir_timer = 0; // Reset the timer
    }
}