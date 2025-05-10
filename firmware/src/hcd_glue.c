#include "tusb_option.h"
#include "tusb.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_hcd.h"
#include "panic.h"

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

void HAL_HCD_MspDeInit(HCD_HandleTypeDef* hcdHandle) {
    (void)hcdHandle;

    __HAL_RCC_USB_OTG_FS_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

    if(__HAL_RCC_PWR_IS_CLK_DISABLED()) {
        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWREx_DisableVddUSB();
        __HAL_RCC_PWR_CLK_DISABLE();
    }
    else {
        HAL_PWREx_DisableVddUSB();
    }

    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
}

// optional hcd configuration, called by tuh_configure()
bool hcd_configure(uint8_t rhport, uint32_t cfg_id, const void* cfg_param) {
    (void)rhport;
    (void)cfg_id;
    (void)cfg_param;
    return true;
}

// Initialize controller to host mode
bool hcd_init(uint8_t rhport, const tusb_rhport_init_t* rh_init) {
    _hhcd_USB.Instance = USB_OTG_FS;
    _hhcd_USB.Init.Host_channels = 8;
    _hhcd_USB.Init.speed = HCD_SPEED_FULL;
    _hhcd_USB.Init.dma_enable = DISABLE;
    _hhcd_USB.Init.phy_itface = HCD_PHY_EMBEDDED;
    _hhcd_USB.Init.Sof_enable = DISABLE;
    if (HAL_HCD_Init(&_hhcd_USB) != HAL_OK) {
        return false;
    }
    HAL_HCD_Start(&_hhcd_USB);

    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    GPIO_InitTypeDef GPIO_Init = {
        .Pin = GPIO_PIN_5,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLDOWN,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = 0,
    };
    HAL_GPIO_Init(GPIOA, &GPIO_Init);

    return true;
}

// De-initialize controller
bool hcd_deinit(uint8_t rhport) {
    (void)rhport;
    HAL_HCD_DeInit(&_hhcd_USB);

    return true;
}

// Interrupt Handler
void OTG_FS_IRQHandler(void) {
    HAL_HCD_IRQHandler(&_hhcd_USB);
}

// unuse callback
void hcd_int_handler(uint8_t rhport, bool in_isr) {
    (void)rhport;
    (void)in_isr;
}

// Enable USB interrupt
void hcd_int_enable(uint8_t rhport) {
    (void)rhport;
    HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(OTG_FS_IRQn);
}

// Disable USB interrupt
void hcd_int_disable(uint8_t rhport) {
    (void)rhport;
    HAL_NVIC_DisableIRQ(OTG_FS_IRQn);
}

// Get frame number (1ms)
uint32_t hcd_frame_number(uint8_t rhport) {
    (void)rhport;
    return HAL_HCD_GetCurrentFrame(&_hhcd_USB);
}

//--------------------------------------------------------------------+
// Port API
//--------------------------------------------------------------------+

static bool _usb_connedted = false;
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd) {
    (void)hhcd;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);
    _usb_connedted = true;
    tuh_int_handler(0, true);
}

void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd) {
    (void)hhcd;
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
    _usb_connedted = false;
    tuh_int_handler(0, false);
}

// Get the current connect status of roothub port
bool hcd_port_connect_status(uint8_t rhport) {
    (void)rhport;
    return _usb_connedted; 
}

// Reset USB bus on the port. Return immediately, bus reset sequence may not 
// be complete. Some port would require hcd_port_reset_end() to be invoked after 
// 10ms to complete the reset sequence.
void hcd_port_reset(uint8_t rhport) {
    (void)rhport;
    HAL_HCD_ResetPort(&_hhcd_USB);
}

// Complete bus reset sequence, may be required by some controllers
void hcd_port_reset_end(uint8_t rhport) {
    (void) rhport;
}

// Get port link speed
tusb_speed_t hcd_port_speed_get(uint8_t rhport) {
    (void)rhport;
    return TUSB_SPEED_FULL;
}

// HCD closes all opened endpoints belong to this device
void hcd_device_close(uint8_t rhport, uint8_t dev_addr) {
    (void) rhport;
    (void) dev_addr;

    for (uint8_t ch = 0; ch < _hhcd_USB.Init.Host_channels; ch++) {
        HAL_HCD_HC_Halt(&_hhcd_USB, ch);
    }
}

//--------------------------------------------------------------------+
// Endpoints API
//--------------------------------------------------------------------+

// Open an endpoint
// return true if successfully opened or endpoint is currently opened
bool hcd_edpt_open(uint8_t rhport, uint8_t daddr, tusb_desc_endpoint_t const *ep_desc) {
    (void) rhport; (void)daddr; (void) ep_desc;
    return false;
}

// Close an endpoint
bool hcd_edpt_close(uint8_t rhport, uint8_t daddr, uint8_t ep_addr) {
  (void) rhport; (void) daddr; (void) ep_addr;
  return false; // TODO not implemented yet
}

// Submit a transfer, when complete hcd_event_xfer_complete() must be invoked
bool hcd_edpt_xfer(uint8_t rhport, uint8_t daddr, uint8_t ep_addr, uint8_t * buffer, uint16_t buflen) {
  (void) rhport; (void) daddr; (void) ep_addr; (void) buffer; (void) buflen;
  return false;
}

// Abort a queued transfer. Note: it can only abort transfer that has not been started
// Return true if a queued transfer is aborted, false if there is no transfer to abort
bool hcd_edpt_abort_xfer(uint8_t rhport, uint8_t dev_addr, uint8_t ep_addr) {
  (void) rhport; (void) dev_addr; (void) ep_addr;
  return false;
}

// Submit a special transfer to send 8-byte Setup Packet, when complete hcd_event_xfer_complete() must be invoked
bool hcd_setup_send(uint8_t rhport, uint8_t daddr, uint8_t const setup_packet[8]) {
  (void) rhport; (void) daddr; (void) setup_packet;
  return false;
}

// clear stall, data toggle is also reset to DATA0
bool hcd_edpt_clear_stall(uint8_t rhport, uint8_t dev_addr, uint8_t ep_addr) {
  (void) rhport; (void) dev_addr; (void) ep_addr;
  return false;
}

void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t idx, uint8_t const* report, uint16_t len) {
    (void)dev_addr; (void)idx; (void)report; (void)len;
}

void tusb_time_delay_ms_api(uint32_t ms) {
    HAL_Delay(ms);
}

uint32_t tusb_time_millis_api(void) {
    return HAL_GetTick();
}
