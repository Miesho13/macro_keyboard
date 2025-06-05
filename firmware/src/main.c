#include "stm32l4xx.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "rcc.h"
#include "stm32l4xx_hal_uart.h"
#include "usb_host.h"
#include "usbh_def.h"
#include "usbh_hid.h"
#include "usbh_hid_keybd.h"
#include "uart.h"

extern USBH_HandleTypeDef hUsbHostFS;
extern ApplicationTypeDef Appli_state;



void SysTick_Handler(void) {
    HAL_IncTick();
}

int main() {
    HAL_Init();
    rcc_init();
    USB_HOST_Init();
    uart_init();
    
    static const char msg[] = "Start..\r\n";
    HAL_UART_Transmit(uart_get_instance(), msg, sizeof(msg), HAL_MAX_DELAY);

    while (1) { 
        USB_HOST_Process();
//         uart_service();
    }
}

