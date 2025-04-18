#ifndef __USB_HARD__ 
#define __USB_HARD__ 

#include "stm32l4xx_hal_hcd.h"

HCD_HandleTypeDef *usb_hard_take();
void usb_hard_init();

#endif
