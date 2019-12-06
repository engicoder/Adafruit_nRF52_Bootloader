#ifndef CONFIG_H
#define CONFIG_H

#include "nrf_gpio.h"

#define LED_CUSTOM
//#define LED_NEOPIXEL
//#define LED_PWM
#define BUTTONS_CUSTOM

#define BLEDIS_MANUFACTURER    "Equinox"
#define BLEDIS_MODEL           "Equinox"

#define UF2_PRODUCT_NAME    "Equinox"
#define UF2_BOARD_ID        "equinox"
#define UF2_VOLUME_LABEL    "EQNXBOOT"
#define UF2_INDEX_URL       "https://www.equinox.com/"

#define LP50XX_EN       NRF_GPIO_PIN_MAP(0,31)
#define LP50XX_SCL      NRF_GPIO_PIN_MAP(0,30)
#define LP50XX_SDA      NRF_GPIO_PIN_MAP(0,29)


#define COLOR_BOOTLOADER_STARTED    0xFF0000
#define COLOR_USB_MOUNTED           0x00FF00
#define COLOR_USB_UNMOUNTED         0xFF0000
#define COLOR_WRITING_STARTED       0xFFFF00
#define COLOR_WRITING_FINISHED      0x00FF00
#define COLOR_BLE_CONNECTED         0x0000FF
#define COLOR_BLE_DISCONNECTED      0xFF00FF


#endif