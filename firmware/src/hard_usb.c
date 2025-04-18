#include "hard_usb.h"
#include "stm32l4xx_hal_hcd.h"
#include "panic.h"
#include "stm32l4xx_hal_cortex.h"

static HCD_HandleTypeDef _hhcd_USB;

void usb_hard_init(void) {
    _hhcd_USB.Instance = USB_OTG_FS;
    _hhcd_USB.Init.Host_channels = 8;
    _hhcd_USB.Init.speed = HCD_SPEED_FULL;
    _hhcd_USB.Init.dma_enable = DISABLE;
    _hhcd_USB.Init.phy_itface = HCD_PHY_EMBEDDED;
    _hhcd_USB.Init.Sof_enable = ENABLE;
    _hhcd_USB.Init.low_power_enable = DISABLE;
    _hhcd_USB.Init.vbus_sensing_enable = ENABLE;
    _hhcd_USB.Init.use_external_vbus = ENABLE;

    if (HAL_HCD_Init(&_hhcd_USB) != HAL_OK) {
        _panic();
    }
    
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

    HAL_HCD_Start(&_hhcd_USB);
}

HCD_HandleTypeDef *usb_hard_take() {
    return &_hhcd_USB;
}









