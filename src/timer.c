#include "stm32f0xx.h"
#include "timer.h"
#include "type.h"

void time_init(void) 
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;

    TIM_TypeDef *time = TIM14;
    time->CR1 = 0;
    time->CNT = 0;
    time->PSC = (u16)((SystemCoreClock / 1000000u) - 1u);
    time->ARR = 0xFFFF;
    time->EGR = TIM_EGR_UG;
    time->CR1 |= TIM_CR1_CEN;
}

void time_delay_us(u32 us)
{
    u16 start = (u16)TIM14->CNT;
    while ((u16)((u16)TIM14->CNT - start) < (u16)us) { }
}
