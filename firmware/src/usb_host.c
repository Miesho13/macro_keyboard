/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file            : usb_host.c
  * @version         : v2.0_Cube
  * @brief           : This file implements the USB Host
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_hid.h"
#include "panic.h"


USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

void Error_Handler() {
    _panic();
}

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

void MX_USB_HOST_Init(void)
{
    if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK)
    {
        Error_Handler();
    }
    if (USBH_RegisterClass(&hUsbHostFS, USBH_HID_CLASS) != USBH_OK)
    {
        Error_Handler();
    }
    if (USBH_Start(&hUsbHostFS) != USBH_OK)
    {
        Error_Handler();
    }
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void)
{
    USBH_Process(&hUsbHostFS);
}

/*
 * user callback definition
 */
static void USBH_UserProcess  (USBH_HandleTypeDef *phost, uint8_t id)
{
      switch(id)
      {
          case HOST_USER_SELECT_CONFIGURATION:
          break;

          case HOST_USER_DISCONNECTION:
          Appli_state = APPLICATION_DISCONNECT;
          break;

          case HOST_USER_CLASS_ACTIVE:
          Appli_state = APPLICATION_READY;

          break;

          case HOST_USER_CONNECTION:
          Appli_state = APPLICATION_START;
          break;

          default:
          break;
      }
}

