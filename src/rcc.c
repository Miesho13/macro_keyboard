#include "rcc.h"
#include "type.h"
#include "stm32f0xx.h"

// STM32F042: max SYSCLK = 48 MHz.
// Use HSI48 (internal 48 MHz oscillator) as system clock.
void rcc_init(void)
{
    RCC->CR2 |= RCC_CR2_HSI48ON;
    while ((RCC->CR2 & RCC_CR2_HSI48RDY) == 0) { }

    FLASH->ACR |= FLASH_ACR_LATENCY;

    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI48;

    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI48) { }

    SystemCoreClockUpdate();
}

