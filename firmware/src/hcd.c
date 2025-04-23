#include "hard_usb.h"
#include "stm32l4xx_hal_hcd.h"
#include "stm32l4xx_hal_gpio.h"
#include "stm32l4xx_hal_rcc.h"
#include "stm32l4xx_hal_pwr.h"
#include "panic.h"
#include "stm32l4xx_hal_cortex.h"
#include "stm32l4xx_hal_rcc_ex.h"

static HCD_HandleTypeDef _hhcd_USB;

void HAL_HCD_MspInit(HCD_HandleTypeDef* hcdHandle) {
    (void)hcdHandle;
    RCC_PeriphCLKInitTypeDef periph_clk = {
        .PeriphClockSelection = RCC_PERIPHCLK_USB,
        .UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1,
        .PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI,
        .PLLSAI1.PLLSAI1M = 1,
        .PLLSAI1.PLLSAI1N = 24,
        .PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7,
        .PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2,
        .PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2,
        .PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK
    };

    if (HAL_RCCEx_PeriphCLKConfig(&periph_clk) != HAL_OK) {
        _panic();
    }
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_init_struct = {
        .Pin = GPIO_PIN_11 | GPIO_PIN_12,
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Speed = GPIO_SPEED_FREQ_VERY_HIGH,
        .Alternate = GPIO_AF10_OTG_FS
    };
    HAL_GPIO_Init(GPIOA, &GPIO_init_struct);

    __HAL_RCC_USB_OTG_FS_CLK_ENABLE();
    if (__HAL_RCC_PWR_IS_CLK_DISABLED()) {
        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWREx_EnableVddUSB();
        __HAL_RCC_PWR_CLK_DISABLE();
    }
    else {
        HAL_PWREx_EnableVddUSB();
    }

    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

void usb_otg_fs_hcd_init(void) {
    _hhcd_USB.Instance = USB_OTG_FS;
    _hhcd_USB.Init.Host_channels = 8;
    _hhcd_USB.Init.speed = HCD_SPEED_FULL;
    _hhcd_USB.Init.dma_enable = DISABLE;
    _hhcd_USB.Init.phy_itface = HCD_PHY_EMBEDDED;
    _hhcd_USB.Init.Sof_enable = DISABLE;
    
    if (HAL_HCD_Init(&_hhcd_USB) != HAL_OK) {
        _panic();
    }

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIO_InitTypeDef GPIO_Init = {
        .Pin = GPIO_PIN_5,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = 0,
    };

    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    HAL_HCD_Start(&_hhcd_USB);
}

HCD_HandleTypeDef *usb_org_fs_hcd_take() {
    return &_hhcd_USB;
}

void OTG_FS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&_hhcd_USB);
}

void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
}






