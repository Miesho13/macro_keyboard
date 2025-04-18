#include "stm32l4xx.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "rcc.h"

void SysTick_Handler(void) {
    HAL_IncTick();
}

int main() {
    HAL_Init();
    rcc_init();

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

    GPIO_InitTypeDef GPIO_Init = {
        .Pin = GPIO_PIN_5,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = 0,
    };

    HAL_GPIO_Init(GPIOA, &GPIO_Init);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

    while (1) { 
        HAL_Delay(1000);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}

