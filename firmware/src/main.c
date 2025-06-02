#include "stm32l4xx.h"
#include "stm32l476xx.h"
#include "stm32l4xx_hal.h"
#include "rcc.h"
#include "usb_host.h"
#include "usbh_def.h"
#include "usbh_hid.h"
#include "usbh_hid_keybd.h"

extern USBH_HandleTypeDef hUsbHostFS;
extern ApplicationTypeDef Appli_state;


void HID_handle() {
    static uint8_t last_key = 0;

    if (USBH_HID_GetDeviceType(&hUsbHostFS) != HID_KEYBOARD) {
        return;
    }

    HID_KEYBD_Info_TypeDef *k_pinfo = USBH_HID_GetKeybdInfo(&hUsbHostFS);
    if (k_pinfo == NULL) {
        return;
    }

    uint8_t ascii = USBH_HID_GetASCIICode(k_pinfo);
    if (ascii != 0 && ascii != last_key) {
        last_key = ascii;

        switch (ascii) {
            case '0' ... '9': {
                break;
            } 

            case '.': {
                break;
            }

            case '\n': {
                break;
            }

            default: {
                break;
            }
        }
    }
}

void SysTick_Handler(void) {
    HAL_IncTick();
}

int main() {
    HAL_Init();
    rcc_init();

    MX_USB_HOST_Init();
    while (1) { 
        MX_USB_HOST_Process();
        if (Appli_state == APPLICATION_READY) {
            HID_handle();
        }
    }
}

