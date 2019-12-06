#ifndef GPIO_NRF5_H
#define GPIO_NRF5_H

#include "gpio.h"

typedef struct gpio_s 
{
    uint32_t line;
    gpio_trigger_handler trigger_handler;
    void* user_data;
} gpio_t;

void gpio_nrf5_construct(gpio_t* gpio, uint32_t line);

#endif /* GPIO_NRF5_H */