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

    tusb_rhport_init_t host_init = {
         .role = TUSB_ROLE_HOST,
         .speed = TUSB_SPEED_AUTO
    };

    tusb_init(0, &host_init);

    while (1) { 
        tuh_task();
    }
}

