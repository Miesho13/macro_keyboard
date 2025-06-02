/**
  ******************************************************************************
  * @file           : Target/usbh_conf.h
  * @version        : v2.0_Cube
  * @brief          : Header for usbh_conf.c file.
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

#ifndef __USBH_CONF__H__
#define __USBH_CONF__H__

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"

#define USBH_MAX_NUM_ENDPOINTS      2U
#define USBH_MAX_NUM_INTERFACES      2U
#define USBH_MAX_NUM_CONFIGURATION      1U
#define USBH_KEEP_CFG_DESCRIPTOR      1U
#define USBH_MAX_NUM_SUPPORTED_CLASS      1U
#define USBH_MAX_SIZE_CONFIGURATION      256U
#define USBH_MAX_DATA_BUFFER      512U
#define USBH_DEBUG_LEVEL      0U
#define USBH_USE_OS      0U
#define HOST_HS 		0
#define HOST_FS 		1

#if (USBH_USE_OS == 1)
  #include "cmsis_os.h"
  #define USBH_PROCESS_PRIO          osPriorityNormal
  #define USBH_PROCESS_STACK_SIZE    ((uint16_t)0)
#endif /* (USBH_USE_OS == 1) */

#define USBH_malloc         malloc

#define USBH_free           free

#define USBH_memset         memset

#define USBH_memcpy         memcpy

#if (USBH_DEBUG_LEVEL > 0U)
#define  USBH_UsrLog(...)   do { \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_UsrLog(...) do {} while (0)
#endif

#if (USBH_DEBUG_LEVEL > 1U)

#define  USBH_ErrLog(...) do { \
                            printf("ERROR: "); \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_ErrLog(...) do {} while (0)
#endif

#if (USBH_DEBUG_LEVEL > 2U)
#define  USBH_DbgLog(...)   do { \
                            printf("DEBUG : "); \
                            printf(__VA_ARGS__); \
                            printf("\n"); \
} while (0)
#else
#define USBH_DbgLog(...) do {} while (0)
#endif

#ifdef __cplusplus
}
#endif
#endif /* __USBH_CONF__H__ */
