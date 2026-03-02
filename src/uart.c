#include "uart.h"
#include "stm32f0xx.h"
#include "gpio_def.h"

void uart_init(void)
{
    RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER  |= (0b10 << (GPIO_USART_TX_PIN * 2)) | 
                     (0b10 << (GPIO_USART_RX_PIN * 2));

    GPIOA->AFR[0] |= (0b0001 << (GPIO_USART_TX_PIN*4)) | 
                     (0b0001 << (GPIO_USART_RX_PIN*4));

    USART2->BRR = (u16)(SystemCoreClock / 115200);
    USART2->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

int  uart_read(u8 *output, u32 output_buffer) 
{
    return 0;
}

void uart_write(u8 *input, u32 size) 
{
    for (u32 count_tx = 0; count_tx < size; count_tx++) {
        while ((USART2->ISR & USART_ISR_TXE) == 0) { }
        USART2->TDR = input[count_tx];
    }
}
