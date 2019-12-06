#include <string.h>
#include "i2c_master.h"

//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
//#include "nrf_pwr_mgmt.h"


static const i2c_result_e event_type_map[] = 
{
    I2C_RESULT_SUCCESS,
    I2C_RESULT_ADDRESS_NACK,
    I2C_RESULT_DATA_NACK,
};

static void
twim_evt_handler(nrfx_twim_evt_t const *p_event, void *p_context)
{
    i2c_master_t* p_master = (i2c_master_t*)p_context;
    if (p_master == NULL)
        return;

    p_master->last_result = event_type_map[p_event->type];
    p_master->busy = false;
}


void
i2c_master_init(i2c_master_t* i2c_master,
                    const nrfx_twim_t* twim, 
                    uint32_t scl, 
                    uint32_t sda)
{
    i2c_master->twim = twim;
    i2c_master->scl = scl;
    i2c_master->sda = sda;
    i2c_master->busy = false;
}

i2c_result_e
i2c_master_enable(i2c_master_t* i2c_master, i2c_speed_e speed)
{
    i2c_result_e result = I2C_RESULT_SUCCESS;

    static nrf_twim_frequency_t freq_map[] =
    {
        NRF_TWIM_FREQ_100K,
        NRF_TWIM_FREQ_250K,
        NRF_TWIM_FREQ_400K,
    };

    const nrfx_twim_config_t config =
    {
      .scl                = i2c_master->scl,
      .sda                = i2c_master->sda,
      .frequency          = freq_map[speed],
      .interrupt_priority = 6,
      .hold_bus_uninit    = false
    };

    nrfx_err_t nrfx_result = nrfx_twim_init(i2c_master->twim,
                                            &config,
                                            twim_evt_handler,
                                            i2c_master);

    if (nrfx_result == NRFX_SUCCESS)
    {
        nrfx_twim_enable(i2c_master->twim);
        i2c_master->busy = false;
   }
    else
    {
        if (nrfx_result == NRFX_ERROR_BUSY)
            result = I2C_RESULT_BUSY;
        else if (nrfx_result == NRFX_ERROR_INVALID_STATE)
            result = I2C_RESULT_INVALID_STATE;
    }

    return result;
}

void
i2c_master_disable(i2c_master_t* i2c_master)
{
    nrfx_twim_uninit(i2c_master->twim);
}

i2c_result_e
i2c_master_transmit(i2c_master_t* i2c_master,
                    uint8_t address,
                    uint8_t* data,
                    uint8_t length,
                    bool stop)
{
    nrfx_err_t nrfx_result;
    i2c_result_e result = I2C_RESULT_SUCCESS;

    if (i2c_master->busy)
        return I2C_RESULT_BUSY;

    i2c_master->busy = true;
    nrfx_result = nrfx_twim_tx(i2c_master->twim, address, data, length, !stop);
    if (nrfx_result == NRFX_SUCCESS)
    {
        while (i2c_master->busy)
            NRFX_DELAY_US(100);
        result = i2c_master->last_result;
    }
    else if (nrfx_result == NRFX_ERROR_BUSY)
        result = I2C_RESULT_BUSY;
    else
        result = I2c_RESULT_INTERNAL;

  return result;
}

i2c_result_e
i2c_master_receive(i2c_master_t* i2c_master,
                   uint8_t address,
                   uint8_t* data,
                   uint8_t length)
{
    nrfx_err_t nrfx_result;
    i2c_result_e result = I2C_RESULT_SUCCESS;

    if (i2c_master->busy)
        return I2C_RESULT_BUSY;
        
    i2c_master->busy = true;
    nrfx_result = nrfx_twim_rx(i2c_master->twim, address, data, length);
    if (nrfx_result == NRFX_SUCCESS)
    {
        while (i2c_master->busy)
            NRFX_DELAY_US(100);
        result = i2c_master->last_result;

    }
    else if (nrfx_result == NRFX_ERROR_BUSY)
        result = I2C_RESULT_BUSY;
    else
        result = I2c_RESULT_INTERNAL;

    return result;
}


i2c_result_e i2c_master_transmit_receive(i2c_master_t* i2c_master,
                                         uint8_t address,
                                         uint8_t* xmit_data,
                                         uint8_t xmit_length,
                                         uint8_t* recv_data,
                                         uint8_t recv_length)
{
    i2c_result_e result = I2C_RESULT_SUCCESS;
    nrfx_err_t nrfx_result;

    if (i2c_master->busy)
        return I2C_RESULT_BUSY;

    nrfx_twim_xfer_desc_t transfer = 
    {
        .type             = NRFX_TWIM_XFER_TXRX,
        .address          = address,
        .p_primary_buf    = xmit_data,
        .primary_length   = xmit_length,
        .p_secondary_buf  = recv_data,
        .secondary_length = recv_length,
    };

    uint32_t flags = NRFX_TWIM_FLAG_TX_NO_STOP;

    i2c_master->busy = true;
    nrfx_result = nrfx_twim_xfer(i2c_master->twim, &transfer, flags);

    if (nrfx_result == NRFX_SUCCESS)
    {
        while (i2c_master->busy)
            NRFX_DELAY_US(100);;
        result = i2c_master->last_result;

    }
    else if (nrfx_result == NRFX_ERROR_BUSY)
        result = I2C_RESULT_BUSY;
    else
        result = I2c_RESULT_INTERNAL;

    return result;
}
