#ifndef __USB_HARD__ 
#define __USB_HARD__ 

#include "stm32l4xx_hal_hcd.h"

void usb_otg_fs_hcd_init(void);
HCD_HandleTypeDef *usb_org_fs_hcd_take(void);

#endif
