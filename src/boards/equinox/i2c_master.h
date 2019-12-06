#pragma once
#include <stdint.h>

#include <string.h>

#include "nrfx.h"
#include "nrf_twim.h"

#include "nrfx_twim.h"

typedef enum i2c_result_e
{
    I2C_RESULT_SUCCESS,
    I2C_RESULT_BUSY,
    I2C_RESULT_OUT_OF_MEMORY,
    I2C_RESULT_TIMEDOUT,
    I2C_RESULT_COMPLETE,
    I2C_RESULT_DATA_NACK,
    I2C_RESULT_ADDRESS_NACK,
    I2C_RESULT_INVALID_STATE,
    I2c_RESULT_INTERNAL,
} i2c_result_e;

typedef enum i2c_speed_e
{
    I2C_SPEED_100,
    I2C_SPEED_250,
    I2C_SPEED_400,
} i2c_speed_e;

typedef struct i2c_master_s
{
    const nrfx_twim_t* twim;
    uint32_t scl;
    uint32_t sda;
    bool busy;
    i2c_result_e last_result;
} i2c_master_t;



void i2c_master_init(i2c_master_t* i2c_master,
                    const nrfx_twim_t* twim,
                    uint32_t scl,
                    uint32_t sda);

i2c_result_e i2c_master_enable(i2c_master_t* i2c_master,
                               i2c_speed_e speed);

void i2c_master_disable(i2c_master_t* i2c_master);

i2c_result_e i2c_master_transmit(i2c_master_t* i2c_master,
                            uint8_t address,
                            uint8_t* data,
                            uint8_t length,
                            bool stop);

i2c_result_e i2c_master_receive(i2c_master_t* i2c_master,
                           uint8_t address,
                           uint8_t* data,
                           uint8_t length);

i2c_result_e i2c_master_transmit_receive(i2c_master_t* i2c_master,
                                         uint8_t address,
                                         uint8_t* xmit_data,
                                         uint8_t xmit_length,
                                         uint8_t* recv_data,
                                         uint8_t recv_length);
