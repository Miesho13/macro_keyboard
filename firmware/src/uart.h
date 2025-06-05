#ifndef __UART__
#define __UART__

#include "./drivers/stm32l4xx_hal_uart.h"

void uart_init();
void uart_service();
UART_HandleTypeDef *uart_get_instance();

#endif
