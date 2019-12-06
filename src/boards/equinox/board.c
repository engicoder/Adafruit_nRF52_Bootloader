
#include "board.h"

#include "i2c_master.h"
#include "gpio.h"
#include "gpio_nrf5.h"
#include "lp50xx.h"

static const nrfx_twim_t lp5018_twim = NRFX_TWIM_INSTANCE(1);

static i2c_master_t i2c_1;
static lp50xx_t lp5018;
static gpio_t lp5018_enable;

static uint16_t timer_count;
//static uint8_t brightness;
static uint16_t blink_interval;

#define SLOW_BLINK_INTERVAL 500
#define FAST_BLINK_INTERVAL 100

#define MAX_BRIGHTNESS 0xFF


void board_init(void)
{
}

void board_teardown(void){}

void board_buttons_init(void){}
bool board_button_pressed(button_e button)
{
    return false;
}

void board_button_teardown(void) {}


void board_led_init(void)
{
    i2c_master_init(&i2c_1,
                    &lp5018_twim,
                    LP50XX_SCL,
                    LP50XX_SDA );

    gpio_nrf5_construct(&lp5018_enable, LP50XX_EN);
    gpio_config_as_push_pull(&lp5018_enable, GPIO_STATUS_LOW);

    lp50xx_init(&lp5018, &i2c_1, &lp5018_enable, LP5018);

    i2c_master_enable(&i2c_1, I2C_SPEED_400);

    lp50xx_enable(&lp5018);

    NRFX_DELAY_US(600);

    lp50xx_standby_exit(&lp5018);


    lp50xx_config_flags_e config = LP50XX_CFG_AUTO_INCR_EN |
                                   LP50XX_CFG_LOG_SCALE_EN |
                                   LP50XX_CFG_PWM_DITHERING_EN |
                                   LP50XX_CFG_POWER_SAVE_EN;

    lp50xx_config_set(&lp5018, config);
}

void board_led_status(led_status_e status)
{
    static uint32_t color_map[] = 
    {
        COLOR_BOOTLOADER_STARTED,
        COLOR_USB_MOUNTED,
        COLOR_USB_UNMOUNTED,
        COLOR_WRITING_STARTED,
        COLOR_WRITING_FINISHED,
        COLOR_BLE_CONNECTED,
        COLOR_BLE_DISCONNECTED,
    };

    uint32_t new_color = color_map[status];

    switch (status) {
        case LED_STATUS_USB_MOUNTED:
            blink_interval = 0;
            break;

        case LED_STATUS_BOOTLOADER_STARTED:
            blink_interval = 0;
            break;

        case LED_STATUS_USB_UNMOUNTED:
            blink_interval = 0;
            new_color = COLOR_USB_UNMOUNTED;
            break;

        case LED_STATUS_WRITING_STARTED:
            blink_interval = FAST_BLINK_INTERVAL;
            break;

        case LED_STATUS_WRITING_FINISHED:
            blink_interval = 0;
            break;

        case LED_STATUS_BLE_CONNECTED:
            blink_interval = 0;
            break;

        case LED_STATUS_BLE_DISCONNECTED:
            blink_interval = 0;
            break;

        default:
            new_color = 0;
            break;
    }

    timer_count = blink_interval;
    lp50xx_led_brightness_set(&lp5018, 0, MAX_BRIGHTNESS);
    lp50xx_led_rgb_set32(&lp5018, 0, new_color);
    lp50xx_led_brightness_set(&lp5018, 1, MAX_BRIGHTNESS);
    lp50xx_led_rgb_set32(&lp5018, 1, new_color);
    
}

void board_led_tick()
{
    if (blink_interval != 0)
    {
        timer_count--;
        if (timer_count == blink_interval/2)
        {
            lp50xx_led_brightness_set(&lp5018, 0, 0);
            lp50xx_led_brightness_set(&lp5018, 1, 0);
        }
        else if (timer_count == 0)
        {
            lp50xx_led_brightness_set(&lp5018, 0, MAX_BRIGHTNESS);
            lp50xx_led_brightness_set(&lp5018, 1, MAX_BRIGHTNESS);
            timer_count = blink_interval;
        }
    }
}

void board_led_teardown(void)
{
    lp50xx_standby_enter(&lp5018);
    lp50xx_disable(&lp5018);

    i2c_master_disable(&i2c_1);
    gpio_disconnect(&lp5018_enable);
}