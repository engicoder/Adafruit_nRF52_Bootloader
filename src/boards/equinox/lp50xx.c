
/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "lp50xx.h"

#define RESET_VALUE 0xFF
#define EN_STARTUP_DELAY 500

typedef bool (*op_handler)(lp50xx_t* driver, i2c_result_e result);

bool on_reset(lp50xx_t* driver, i2c_result_e result);
bool on_standby_enter(lp50xx_t* driver, i2c_result_e result);
bool on_standby_exit(lp50xx_t* driver, i2c_result_e result);
bool on_read_register(lp50xx_t* driver, i2c_result_e result);
bool on_write_register(lp50xx_t* driver, i2c_result_e result);

static const lp50xx_reg_info_s lp5009_reg_info = 
{
    3,
    LP5012_BANK_BRIGHTNESS,
    LP5012_BANK_A_COLOR,
    LP5012_LED0_BRIGHTNESS,
    LP5012_OUT0_COLOR,
    LP5012_RESET,
};

static const lp50xx_reg_info_s lp5012_reg_info = 
{
    4,
    LP5012_BANK_BRIGHTNESS,
    LP5012_BANK_A_COLOR,
    LP5012_LED0_BRIGHTNESS,
    LP5012_OUT0_COLOR,
    LP5012_RESET,
};

static const lp50xx_reg_info_s lp5018_reg_info = 
{
    6,
    LP5024_BANK_BRIGHTNESS,
    LP5024_BANK_A_COLOR,
    LP5024_LED0_BRIGHTNESS,
    LP5024_OUT0_COLOR,
    LP5024_RESET,
};

static const lp50xx_reg_info_s lp5024_reg_info = 
{
    8,
    LP5024_BANK_BRIGHTNESS,
    LP5024_BANK_A_COLOR,
    LP5024_LED0_BRIGHTNESS,
    LP5024_OUT0_COLOR,
    LP5024_RESET,
};

static const lp50xx_reg_info_s lp5030_reg_info = 
{
    10,
    LP5036_BANK_BRIGHTNESS,
    LP5036_BANK_A_COLOR,
    LP5036_LED0_BRIGHTNESS,
    LP5036_OUT0_COLOR,
    LP5036_RESET,
};

static const lp50xx_reg_info_s lp5036_reg_info = 
{
    12,
    LP5036_BANK_BRIGHTNESS,
    LP5036_BANK_A_COLOR,
    LP5036_LED0_BRIGHTNESS,
    LP5036_OUT0_COLOR,
    LP5036_RESET,
};

static const lp50xx_reg_info_s* part_reg_info[] =
{
    &lp5009_reg_info,
    &lp5012_reg_info,
    &lp5018_reg_info,
    &lp5024_reg_info,
    &lp5030_reg_info,
    &lp5036_reg_info,
};


static void
device_enable(lp50xx_t* driver)
{
    gpio_set(driver->enable_line);
}

static void
device_disable(lp50xx_t* driver)
{
    gpio_clear(driver->enable_line);
}

static lp50xx_result_e
device_write(lp50xx_t* driver, 
             lp50xx_address_e address,
             uint8_t reg,
             uint8_t value)
{
    i2c_result_e i2c_result;
    lp50xx_result_e result = LP50XX_RESULT_SUCCESS;

    static uint8_t buffer[2];
    uint8_t length = 2;

    buffer[0] = reg;
    buffer[1] = value;


    i2c_result = i2c_master_transmit(driver->i2c,
                                     driver->address,
                                     buffer,
                                     length,
                                     true);

    result = (i2c_result == I2C_RESULT_SUCCESS) ?
                LP50XX_RESULT_SUCCESS : LP50XX_RESULT_FAILED;

    return result;
}

static lp50xx_result_e
device_write_sequential(lp50xx_t* driver, 
                        lp50xx_address_e address,
                        uint8_t reg,
                        uint8_t* data,
                        uint8_t length)
{
    i2c_result_e i2c_result;
    lp50xx_result_e result = LP50XX_RESULT_SUCCESS;

    static uint8_t buffer[64];
    
    buffer[0] = reg;
    memcpy(&buffer[1], data, length);
    

    i2c_result = i2c_master_transmit(driver->i2c,
                                 address,
                                 buffer,
                                 length + 1,
                                 true);


    result = (i2c_result == I2C_RESULT_SUCCESS) ?
                LP50XX_RESULT_SUCCESS : LP50XX_RESULT_FAILED;

    return result;
}


static lp50xx_result_e
device_read(lp50xx_t* driver, 
            lp50xx_address_e address,
            uint8_t reg,
            uint8_t* rd_buffer,
            uint8_t rd_length)
{
    i2c_result_e i2c_result;
    lp50xx_result_e result = LP50XX_RESULT_SUCCESS;

    static uint8_t wr_buffer[2];
    uint8_t wr_length = 1;

    wr_buffer[0] = reg;

    i2c_result = i2c_master_transmit(driver->i2c,
                                 address,
                                 wr_buffer, 
                                 wr_length,
                                 true);
    if (i2c_result == I2C_RESULT_SUCCESS)
    {
        i2c_result = i2c_master_receive(driver->i2c,
                                        address,
                                        rd_buffer, 
                                        rd_length);
    }

    result = (i2c_result == I2C_RESULT_SUCCESS) ?
                LP50XX_RESULT_SUCCESS : LP50XX_RESULT_FAILED;

    return result;
}

void lp50xx_init(lp50xx_t* driver, 
                 i2c_master_t* i2c, 
                 gpio_t* enable_line,
                 lp50xx_part_e part_id)
{
    driver->part_id = part_id;
    driver->reg_info = part_reg_info[part_id];
    driver->address = LP50XX_ADDRESS_DEFAULT;
    driver->i2c = i2c;
    driver->enable_line = enable_line;

    driver->state = LP50XX_STATE_SHUTDOWN;
};

lp50xx_result_e 
lp50xx_reset(lp50xx_t* driver)
{
    if (driver->state != LP50XX_STATE_STANDBY || 
        driver->state != LP50XX_STATE_NORMAL )
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver, 
                                          driver->address,
                                          driver->reg_info->reset,
                                          RESET_VALUE);

    driver->state = LP50XX_STATE_STANDBY;

    return result;
}

void lp50xx_enable(lp50xx_t* driver)
{
    if (driver->state != LP50XX_STATE_SHUTDOWN)
        return;

    device_enable(driver);

    driver->state = LP50XX_STATE_INITIALIZATION;

    uint8_t config;
    device_read(driver, 
                driver->address,
                LP50XX_DEVICE_CONFIG0,
                &config,
                1);

    driver->state = LP50XX_STATE_STANDBY;
}

void lp50xx_disable(lp50xx_t* driver)
{
    device_disable(driver);
    driver->state = LP50XX_STATE_SHUTDOWN;
}

void lp50xx_state_query(lp50xx_t* driver)
{
    gpio_status_e en_state = gpio_read(driver->enable_line);
    if (en_state == GPIO_STATUS_LOW)
        driver->state = LP50XX_STATE_SHUTDOWN;
    else
    {
        uint8_t config;
        device_read(driver, 
                    driver->address,
                    LP50XX_DEVICE_CONFIG0,
                    &config,
                    1);
    }
}

lp50xx_state_e 
lp50xx_state_get(lp50xx_t* driver)
{
    return driver->state;
}


void lp50xx_address_set(lp50xx_t* driver, lp50xx_address_e address)
{
    driver->address = address;
}


lp50xx_result_e 
lp50xx_standby_enter(lp50xx_t* driver)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;


    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          LP50XX_DEVICE_CONFIG0,
                                          ~LP50XX_CONFIG0_CHIP_EN);        
    return result;
}

lp50xx_result_e 
lp50xx_standby_exit(lp50xx_t* driver)
{
    if (driver->state != LP50XX_STATE_STANDBY)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          LP50XX_DEVICE_CONFIG0,
                                          LP50XX_CONFIG0_CHIP_EN);

    driver->state = LP50XX_STATE_NORMAL;

    return result;
}

lp50xx_result_e 
lp50xx_config_set(lp50xx_t* driver, lp50xx_config_flags_e config)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          LP50XX_DEVICE_CONFIG1,
                                          config);
    return result;
}
/*
lp50xx_config_flags_e 
lp50xx_config_get(lp50xx_t* driver, lp50xx_config_flags_e config)
{
    uint8_t value;
    device_read(driver->i2c,
                 driver->address,
                 LP50XX_DEVICE_CONFIG1,
                 &value);
    return (lp50xx_config_flags_e)value;
}
*/

lp50xx_result_e
lp50xx_led_mode_set(lp50xx_t* driver, uint8_t led, lp50xx_led_mode_e mode)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          LP50XX_LED_CONFIG0,
                                          mode << led);
    return result;
}
 
/*
lp50xx_led_mode_e lp50xx_led_mode_get(lp50xx_t* driver, uint8_t led)
{
    uint8_t value;
    device_read(driver->i2c,
                 driver->address,
                 LP50XX_LED_CONFIG0,
                 &value);
    return (lp50xx_led_mode_e)(value & (1 << led));
}
*/

lp50xx_result_e
lp50xx_bank_brightness_set(lp50xx_t* driver, uint8_t brightness)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          driver->reg_info->bank_brightness,
                                          brightness);
    return result;
}

/*
uint8_t lp50xx_bank_brightness_get(lp50xx_t* driver)
{
    return 0;
}
*/

lp50xx_result_e
lp50xx_bank_color_set(lp50xx_t* driver, lp50xx_bank_e bank, uint8_t color)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          driver->reg_info->bank_color + bank,
                                          color);
    return result;
}

lp50xx_result_e
lp50xx_led_brightness_set(lp50xx_t* driver, uint8_t led, uint8_t brightness)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          driver->reg_info->led_brightness + led,
                                          brightness);
    return result;
}

lp50xx_result_e
lp50xx_out_color_set(lp50xx_t* driver, uint8_t out, uint8_t color)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    lp50xx_result_e result = device_write(driver,
                                          driver->address,
                                          driver->reg_info->out_color + out,
                                          color);
    return result;
}

lp50xx_result_e
lp50xx_led_rgb_set(lp50xx_t* driver, 
                   uint8_t led, 
                   uint8_t red, uint8_t green, uint8_t blue)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    static uint8_t data[3];
    
    uint8_t reg = driver->reg_info->out_color + 3 * led;
    data[0] = green;
    data[1] = blue;
    data[2] = red;
    
    lp50xx_result_e result = device_write_sequential(driver, 
                                                     driver->address,
                                                     reg,
                                                     data, 
                                                     sizeof(data));
    return result;
}

    lp50xx_result_e 
    lp50xx_led_rgb_set32(lp50xx_t* driver, uint8_t led, uint32_t rgb_color)
    {
        lp50xx_result_e result = lp50xx_led_rgb_set(driver,
                                                    led,
                                                    (rgb_color >> 16) & 0xff,
                                                    (rgb_color >> 8) & 0xff,
                                                    rgb_color & 0xff);
        return result;
    }

lp50xx_result_e
lp50xx_led_rgba_set(lp50xx_t* driver, uint8_t led, uint32_t rgba_color)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    uint8_t alpha = rgba_color & 0xff;
    lp50xx_result_e result = lp50xx_led_brightness_set(driver, led, alpha);

    if (result == LP50XX_RESULT_SUCCESS)
    {
        uint8_t red = (rgba_color >> 24) & 0xff;
        uint8_t green = (rgba_color >> 16) & 0xff;
        uint8_t blue = (rgba_color >> 8) & 0xff;
        lp50xx_led_rgb_set(driver, led, red, green, blue);
    }
    return result;
}

lp50xx_result_e
lp50xx_led_argb_set(lp50xx_t* driver, uint8_t led, uint32_t argb_color)
{
    if (driver->state != LP50XX_STATE_NORMAL)
        return LP50XX_RESULT_INVALID_STATE;

    uint8_t alpha = (argb_color >> 24) & 0xff;
    lp50xx_result_e result = lp50xx_led_brightness_set(driver, led, alpha);

    if (result == LP50XX_RESULT_SUCCESS)
    {
        uint8_t red = (argb_color >> 16) & 0xff;
        uint8_t green = (argb_color >> 8) & 0xff;
        uint8_t blue = argb_color & 0xff;
        lp50xx_led_rgb_set(driver, led, red, green, blue);
    }
    return result;
}

