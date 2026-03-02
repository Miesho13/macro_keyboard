#ifndef __GPIO__
#define __GPIO__

#include "stm32f0xx.h"
#include "gpio_def.h"
#include "type.h"

void gpio_init(void);
void gpio_togle(GPIO_TypeDef *gpio_port, u32 pin);
void gpio_height(GPIO_TypeDef *gpio_port, u32 pin);
void gpio_low(GPIO_TypeDef *gpio_port, u32 pin);
u8   gpio_read(GPIO_TypeDef *gpio_port, u32 pin);

#endif // __GPIO__
