#include "stm32f0xx.h"
#include "rcc.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"

void init(void) 
{
    SystemInit();
    SystemCoreClockUpdate();

    rcc_init();
    gpio_init();
    time_init();
    uart_init();
}

int main(void)
{
    init();

    while (1) { 
        gpio_togle(GPIOB, 3);

        u8 msg[] = "Hello Ghuciapko\r\n";
        uart_write(msg, sizeof(msg));

        time_delay_us(100);
    }
}
