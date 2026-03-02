#include "gpio.h"

void gpio_init(void)
{
    /* SETUP GREAN DIODE */ {
        RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
        GPIO_GREEN_LED_DIODE_PORT->MODER   |= (1 << (GPIO_GREEN_LED_DIODE_PIN * 2));
        GPIO_GREEN_LED_DIODE_PORT->OSPEEDR |= (0b11 << (GPIO_GREEN_LED_DIODE_PIN * 2));
    }
}

void gpio_togle(GPIO_TypeDef *gpio_port, u32 pin)
{
    gpio_port->ODR ^= (1u << pin);
}

void gpio_height(GPIO_TypeDef *gpio_port, u32 pin)
{
    gpio_port->BSRR = (1u << pin);
}

void gpio_low(GPIO_TypeDef *gpio_port, u32 pin)
{
    gpio_port->BSRR = (1u << (pin + 16u));
}

u8 gpio_read(GPIO_TypeDef *gpio_port, u32 pin)
{
    return (u8)((gpio_port->IDR >> pin) & 1u);
}
