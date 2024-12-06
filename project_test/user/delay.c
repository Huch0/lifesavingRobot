#include "delay.h"

void delay(int time)
{
    int i;
    for (i = 0; i < time; i++)
    {
    }
}

void delay_us(uint32_t us) {
    uint32_t count = us * (SystemCoreClock / 1000000) / 5;
    while (count--) {
        __asm("nop");
    }
}