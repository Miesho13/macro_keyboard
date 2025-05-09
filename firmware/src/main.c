#include "stm32l4xx.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "rcc.h"
#include "tusb.h"

void SysTick_Handler(void) {
    HAL_IncTick();
}

int main() {
    HAL_Init();
    rcc_init();
    // usb_otg_fs_hcd_init();

    tuh_init(0);

    while (1) { 
        tuh_task();
    }
}

