#include "gpio_nrf5.h"
#include "nrf_error.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"

static gpio_t* pin_map[2][32] = {{0},{0}};
#define GET_PORT(x) (x >> 5) 
#define GET_PIN(x) (x & 0xF)

static nrf_gpio_pin_pull_t pull_map[] = 
{
    NRF_GPIO_PIN_PULLDOWN,
    NRF_GPIO_PIN_PULLUP,
    NRF_GPIO_PIN_NOPULL,
};

static nrf_gpiote_polarity_t sense_map[] = 
{
    NRF_GPIOTE_POLARITY_HITOLO,
    NRF_GPIOTE_POLARITY_LOTOHI,
    NRF_GPIOTE_POLARITY_TOGGLE,
};

void gpio_nrf5_construct(gpio_t* gpio, uint32_t line)
{
    gpio->line = line;
}


void gpio_config_as_input(gpio_t* gpio, gpio_pull_e pull)
{
    gpio->trigger_handler = NULL;

    nrf_gpio_cfg(gpio->line,
        NRF_GPIO_PIN_DIR_INPUT,
        NRF_GPIO_PIN_INPUT_CONNECT,
        pull_map[pull],
        NRF_GPIO_PIN_S0S1,
        NRF_GPIO_PIN_NOSENSE);    
}

void gpio_config_as_push_pull(gpio_t* gpio, gpio_status_e initial_state)
{    
    gpio->trigger_handler = NULL;
    
    if (initial_state == GPIO_STATUS_HIGH)
        nrf_gpio_pin_set(gpio->line);
    else
        nrf_gpio_pin_clear(gpio->line);

    nrf_gpio_cfg(gpio->line,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        NRF_GPIO_PIN_NOPULL,
        NRF_GPIO_PIN_S0S1,
        NRF_GPIO_PIN_NOSENSE);
}

void 
gpio_config_as_open_drain(gpio_t* gpio, 
                        gpio_status_e initial_state,
                        gpio_pull_e pull)
{
    gpio->trigger_handler = NULL;

    if (initial_state == GPIO_STATUS_HIGH)
        nrf_gpio_pin_set(gpio->line);
    else
        nrf_gpio_pin_clear(gpio->line);

    nrf_gpio_cfg(gpio->line,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        pull_map[pull],
        NRF_GPIO_PIN_S0D1,
        NRF_GPIO_PIN_NOSENSE);
}

void gpio_config_as_open_source(gpio_t* gpio, 
                                gpio_status_e initial_state, 
                                gpio_pull_e pull)
{
    gpio->trigger_handler = NULL;

    if (initial_state == GPIO_STATUS_HIGH)
        nrf_gpio_pin_set(gpio->line);
    else
        nrf_gpio_pin_clear(gpio->line);

    nrf_gpio_cfg(gpio->line,
        NRF_GPIO_PIN_DIR_OUTPUT,
        NRF_GPIO_PIN_INPUT_DISCONNECT,
        pull_map[pull],
        NRF_GPIO_PIN_D0S1,
        NRF_GPIO_PIN_NOSENSE);
}

void gpiote_in_event_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    gpio_t* gpio = pin_map[GET_PORT(pin)][GET_PIN(pin)];
    if (gpio != NULL && gpio->trigger_handler != NULL)
    {
        gpio_status_e status = gpio_read(gpio);
        gpio->trigger_handler(gpio, status, gpio->user_data);
    }
}

void gpio_add_trigger(gpio_t* gpio, 
                      gpio_trigger_e trigger, 
                      gpio_trigger_handler handler,
                      void* user_data)
{
    nrfx_err_t err_code;

    gpio->trigger_handler = handler;
    gpio->user_data = user_data;

    if(!nrfx_gpiote_is_init())
    {
        err_code = nrfx_gpiote_init();
    }
    
    NRF_GPIO_Type * reg = nrf_gpio_pin_port_decode(&gpio->line);    

    nrf_gpio_pin_pull_t pull =  
        (nrf_gpio_pin_pull_t)((reg->PIN_CNF[gpio->line] &
            GPIO_PIN_CNF_PULL_Msk) >> GPIO_PIN_CNF_PULL_Pos);

    nrfx_gpiote_in_config_t config = 
    {
        .sense = sense_map[trigger],
        .hi_accuracy = false,
        .is_watcher = false,
        .pull = pull,
    };
    err_code = nrfx_gpiote_in_init(gpio->line, &config, gpiote_in_event_handler);
    if(err_code != NRF_SUCCESS)
    {
      // Error during initialization: NRFX_ERROR_INVALID_STATE or NRFX_ERROR_NO_MEM
    }

    pin_map[GET_PORT(gpio->line)][GET_PIN(gpio->line)] = gpio;
    nrfx_gpiote_in_event_enable(gpio->line, true);
}


void gpio_disconnect(gpio_t* gpio)
{
    nrf_gpio_cfg_default(gpio->line);   
}

void gpio_set(gpio_t* gpio)
{
    nrf_gpio_pin_set(gpio->line);
}

void gpio_clear(gpio_t* gpio)
{
    nrf_gpio_pin_clear(gpio->line);
}

gpio_status_e gpio_read(gpio_t* gpio)
{
    gpio_status_e status;

    uint32_t value;

    if (gpio->trigger_handler == NULL)
        value = nrfx_gpiote_in_is_set(gpio->line);
    else
        value = nrf_gpio_pin_read(gpio->line);

    if (value == 0)
        status = GPIO_STATUS_LOW;
    else
        status = GPIO_STATUS_HIGH;

    return status;
}

