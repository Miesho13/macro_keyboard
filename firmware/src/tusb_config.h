#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#define CFG_TUSB_MCU OPT_MCU_STM32L4
#define CFG_TUSB_OS OPT_OS_NONE  // or FREERTOS if you're using it
#define CFG_TUSB_RHPORT0_MODE OPT_MODE_HOST
#define CFG_TUSB_HOST_PORT 0

#define CFG_TUH_ENUMERATION_BUFSIZE 256

// Enable class drivers
#define CFG_TUH_HID         1
#define CFG_TUH_MSC         1
#define CFG_TUH_CDC         1
#define CFG_TUH_VENDOR      0

#endif
