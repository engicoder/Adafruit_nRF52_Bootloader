#ifndef LP50XX_H_
#define LP50XX_H_

#include <stdint.h>
#include <stdbool.h>

#include "gpio.h"
#include "i2c_master.h"





#define LP50XX_DEVICE_CONFIG0                   0x00
#define LP50XX_DEVICE_CONFIG1                   0x01
#define LP50XX_LED_CONFIG0			0x02

/* LP5009 and LP5012 registers */

#define LP5012_BANK_BRIGHTNESS                  0x03
#define LP5012_BANK_A_COLOR			0x04
#define LP5012_BANK_B_COLOR			0x05
#define LP5012_BANK_C_COLOR			0x06
#define LP5012_LED0_BRIGHTNESS                  0x07
#define LP5012_LED1_BRIGHTNESS                  0x08
#define LP5012_LED2_BRIGHTNESS                  0x09
#define LP5012_LED3_BRIGHTNESS                  0x0a

#define LP5012_OUT0_COLOR			0x0b
#define LP5012_OUT1_COLOR			0x0c
#define LP5012_OUT2_COLOR			0x0d
#define LP5012_OUT3_COLOR			0x0e
#define LP5012_OUT4_COLOR			0x0f
#define LP5012_OUT5_COLOR			0x10
#define LP5012_OUT6_COLOR			0x11
#define LP5012_OUT7_COLOR			0x12
#define LP5012_OUT8_COLOR			0x13
#define LP5012_OUT9_COLOR			0x14
#define LP5012_OUT10_COLOR			0x15
#define LP5012_OUT11_COLOR			0x16

#define LP5012_RESET				0x17

/* P5018 and LP5024 registers */

#define LP5024_BANK_BRIGHTNESS                  0x03
#define LP5024_BANK_A_COLOR			0x04
#define LP5024_BANK_B_COLOR			0x05
#define LP5024_BANK_C_COLOR			0x06
#define LP5024_LED0_BRIGHTNESS                  0x07
#define LP5024_LED1_BRIGHTNESS                  0x08
#define LP5024_LED2_BRIGHTNESS                  0x09
#define LP5024_LED3_BRIGHTNESS                  0x0a
#define LP5024_LED4_BRIGHTNESS                  0x0b
#define LP5024_LED5_BRIGHTNESS                  0x0c
#define LP5024_LED6_BRIGHTNESS                  0x0d
#define LP5024_LED7_BRIGHTNESS                  0x0e

#define LP5024_OUT0_COLOR			0x0f
#define LP5024_OUT1_COLOR			0x10
#define LP5024_OUT2_COLOR			0x11
#define LP5024_OUT3_COLOR			0x12
#define LP5024_OUT4_COLOR			0x13
#define LP5024_OUT5_COLOR			0x14
#define LP5024_OUT6_COLOR			0x15
#define LP5024_OUT7_COLOR			0x16
#define LP5024_OUT8_COLOR			0x17
#define LP5024_OUT9_COLOR			0x18
#define LP5024_OUT10_COLOR			0x19
#define LP5024_OUT11_COLOR			0x1a
#define LP5024_OUT12_COLOR			0x1b
#define LP5024_OUT13_COLOR			0x1c
#define LP5024_OUT14_COLOR			0x1d
#define LP5024_OUT15_COLOR			0x1e
#define LP5024_OUT16_COLOR			0x1f
#define LP5024_OUT17_COLOR			0x20
#define LP5024_OUT18_COLOR			0x21
#define LP5024_OUT19_COLOR			0x22
#define LP5024_OUT20_COLOR			0x23
#define LP5024_OUT21_COLOR			0x24
#define LP5024_OUT22_COLOR			0x25
#define LP5024_OUT23_COLOR			0x26
#define LP5024_RESET				0x27

/* LP5030 and LP5036 registers */
#define LP5036_LED_CONFIG1			0x03
#define LP5036_BANK_BRIGHTNESS		0x04
#define LP5036_BANK_A_COLOR			0x05
#define LP5036_BANK_B_COLOR			0x06
#define LP5036_BANK_C_COLOR			0x07
#define LP5036_LED0_BRIGHTNESS		0x08
#define LP5036_LED1_BRIGHTNESS		0x09
#define LP5036_LED2_BRIGHTNESS		0x0a
#define LP5036_LED3_BRIGHTNESS		0x0b
#define LP5036_LED4_BRIGHTNESS		0x0c
#define LP5036_LED5_BRIGHTNESS		0x0d
#define LP5036_LED6_BRIGHTNESS		0x0e
#define LP5036_LED7_BRIGHTNESS		0x0f
#define LP5036_LED8_BRIGHTNESS		0x10
#define LP5036_LED9_BRIGHTNESS		0x11
#define LP5036_LED10_BRIGHTNESS		0x12
#define LP5036_LED11_BRIGHTNESS		0x13

#define LP5036_OUT0_COLOR			0x14
#define LP5036_OUT1_COLOR			0x15
#define LP5036_OUT2_COLOR			0x16
#define LP5036_OUT3_COLOR			0x17
#define LP5036_OUT4_COLOR			0x18
#define LP5036_OUT5_COLOR			0x19
#define LP5036_OUT6_COLOR			0x1a
#define LP5036_OUT7_COLOR			0x1b
#define LP5036_OUT8_COLOR			0x1c
#define LP5036_OUT9_COLOR			0x1d
#define LP5036_OUT10_COLOR			0x1e
#define LP5036_OUT11_COLOR			0x1f
#define LP5036_OUT12_COLOR			0x20
#define LP5036_OUT13_COLOR			0x21
#define LP5036_OUT14_COLOR			0x22
#define LP5036_OUT15_COLOR			0x23
#define LP5036_OUT16_COLOR			0x24
#define LP5036_OUT17_COLOR			0x25
#define LP5036_OUT18_COLOR			0x26
#define LP5036_OUT19_COLOR			0x27
#define LP5036_OUT20_COLOR			0x28
#define LP5036_OUT21_COLOR			0x29
#define LP5036_OUT22_COLOR			0x2a
#define LP5036_OUT23_COLOR			0x2b
#define LP5036_OUT24_COLOR			0x2c
#define LP5036_OUT25_COLOR			0x2d
#define LP5036_OUT26_COLOR			0x2e
#define LP5036_OUT27_COLOR			0x2f
#define LP5036_OUT28_COLOR			0x30
#define LP5036_OUT29_COLOR			0x31
#define LP5036_OUT30_COLOR			0x32
#define LP5036_OUT31_COLOR			0x33
#define LP5036_OUT32_COLOR			0x34
#define LP5036_OUT33_COLOR			0x35
#define LP5036_OUT34_COLOR			0x36
#define LP5036_OUT35_COLOR			0x37
#define LP5036_RESET				0x38


#define LP50XX_CONFIG0_CHIP_EN      (1 << 6)

#define LP50XX_WRITE_BUFFER_SIZE 64
#define LP50XX_READ_BUFFER_SIZE  64

typedef enum lp50xx_result_e
{
    LP50XX_RESULT_SUCCESS,
    LP50XX_RESULT_BUSY,
    LP50XX_RESULT_FAILED,
    LP50XX_RESULT_INVALID_STATE,
} lp50xx_result_e;


typedef enum lp50xx_state_e
{
    LP50XX_STATE_SHUTDOWN,
    LP50XX_STATE_INITIALIZATION,
    LP50XX_STATE_STANDBY,
    LP50XX_STATE_NORMAL,
} lp50xx_state_e;

typedef enum lp50xx_operation_e
{
    LP50XX_OP_RESET,
    LP50XX_OP_STANDBY_ENTER,
    LP50XX_OP_STANDBY_EXIT,
    LP50XX_OP_READ_REGISTER,
    LP50XX_OP_WRITE_REGISTER,
} lp50xx_operation_e;

typedef enum lp50xx_address_e
{
    LP50XX_ADDRESS_00 = 0x28,
    LP50XX_ADDRESS_01 = 0x29,
    LP50XX_ADDRESS_10 = 0x2A,
    LP50XX_ADDRESS_11 = 0x2B,
    LP50XX_ADDRESS_BROADCAST = 0x3c,
    LP50XX_ADDRESS_DEFAULT = LP50XX_ADDRESS_BROADCAST,
} lp50xx_address_e;


typedef struct lp50xx_reg_info_s
{
    uint8_t max_leds;
    uint8_t bank_brightness;
    uint8_t bank_color;
    uint8_t led_brightness;
    uint8_t out_color;
    uint8_t reset;
} lp50xx_reg_info_s;

typedef enum lp50xx_part_e
{
    LP5009,
    LP5012,
    LP5018,
    LP5024,
    LP5030,
    LP5036,
} lp50xx_part_e;



typedef enum lp50xx_config_flags_e
{
    LP50XX_CFG_LED_GLOBAL_OFF       = (1 << 0),
    LP50XX_CFG_MAX_CURRENT_OPTION   = (1 << 1),
    LP50XX_CFG_PWM_DITHERING_EN     = (1 << 2),
    LP50XX_CFG_AUTO_INCR_EN         = (1 << 3),
    LP50XX_CFG_POWER_SAVE_EN        = (1 << 4),
    LP50XX_CFG_LOG_SCALE_EN         = (1 << 5),
    LP50XX_CFG_DEFAULT              = (LP50XX_CFG_AUTO_INCR_EN |
                                       LP50XX_CFG_LOG_SCALE_EN |
                                       LP50XX_CFG_PWM_DITHERING_EN |
                                       LP50XX_CFG_POWER_SAVE_EN),
} lp50xx_config_flags_e;




typedef enum lp50xx_led_mode_e
{
    LP50XX_LED_MODE_INDEPENDENT,
    LP50XX_LED_MODE_BANK,
    LP50XX_LED_MODE_DEFAULT = LP50XX_LED_MODE_INDEPENDENT,
} lp50xx_led_mode_e;

typedef enum lp50xx_bank_e
{
    lp50xx_bank_a,
    lp50xx_bank_b,
    lp50xx_bank_c,
} lp50xx_bank_e;


typedef enum lp50xx_led_e
{
    lp50xx_led_0,
    lp50xx_led_1,
    lp50xx_led_2,
    lp50xx_led_3,
    lp50xx_led_4,
    lp50xx_led_5,
    lp50xx_led_6,
    lp50xx_led_7,
} lp50xx_led_e;

typedef struct lp50xx_s
{
    i2c_master_t* i2c; 
    gpio_t*       enable_line; 
    lp50xx_part_e  part_id;
    const lp50xx_reg_info_s* reg_info;
    lp50xx_address_e address;
    lp50xx_state_e state;
} lp50xx_t;


/* Assumes i2c_master is aleady configured and enabled */
void lp50xx_init(lp50xx_t* driver, 
                 i2c_master_t* i2c, 
                 gpio_t* enable_line, 
                 lp50xx_part_e part_id);

lp50xx_result_e lp50xx_reset(lp50xx_t* driver);
void lp50xx_enable(lp50xx_t* driver);
void lp50xx_disable(lp50xx_t* driver);

void lp50xx_address_set(lp50xx_t* driver, lp50xx_address_e address);

void lp50xx_state_query(lp50xx_t* driver);
lp50xx_state_e lp50xx_state_get(lp50xx_t* driver);

lp50xx_result_e lp50xx_standby_enter(lp50xx_t* driver);
lp50xx_result_e lp50xx_standby_exit(lp50xx_t* driver);

lp50xx_result_e lp50xx_config_set(lp50xx_t* driver, lp50xx_config_flags_e config);

lp50xx_result_e lp50xx_led_mode_set(lp50xx_t* driver, uint8_t led, lp50xx_led_mode_e mode);


lp50xx_result_e lp50xx_bank_brightness_set(lp50xx_t* driver, uint8_t brightness);
lp50xx_result_e lp50xx_bank_color_set(lp50xx_t* driver, lp50xx_bank_e bank, uint8_t color);

lp50xx_result_e lp50xx_led_brightness_set(lp50xx_t* driver, uint8_t led, uint8_t brightness);
lp50xx_result_e lp50xx_out_color_set(lp50xx_t* driver, uint8_t out, uint8_t color);


lp50xx_result_e lp50xx_led_rgb_set(lp50xx_t* driver, uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
lp50xx_result_e lp50xx_led_rgb_set32(lp50xx_t* driver, uint8_t led, uint32_t rgb_color);
lp50xx_result_e lp50xx_led_rgba_set(lp50xx_t* driver, uint8_t led, uint32_t rgba_color);
lp50xx_result_e lp50xx_led_argb_set(lp50xx_t* driver, uint8_t led, uint32_t argb_color);


#endif /* LP50XX_H_ */
