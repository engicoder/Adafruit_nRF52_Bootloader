#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

typedef enum gpio_status_e
{
    GPIO_STATUS_LOW,
    GPIO_STATUS_HIGH,
} gpio_status_e;

typedef enum gpio_mode_e
{
    GPIO_MODE_INPUT,
    GPIO_MODE_PUSH_PULL,
    GPIO_MODE_OPEN_DRAIN,
} gpio_mode_e;

typedef enum gpio_pull_e
{

    GPIO_PULL_DOWN,
    GPIO_PULL_UP,
    GPIO_PULL_NONE,
} gpio_pull_e;

typedef enum gpio_trigger_e
{
    GPIO_TRIGGER_FALLING_EDGE,
    GPIO_TRIGGER_RISING_EDGE,
    GPIO_TRIGGER_TOGGLE,
} gpio_trigger_e;

typedef struct gpio_s gpio_t;

typedef void (*gpio_trigger_handler)(gpio_t* gpio, gpio_status_e status, void* user_data);

void gpio_config_as_input(gpio_t* gpio, gpio_pull_e pull);
void gpio_config_as_push_pull(gpio_t* gpio, gpio_status_e initial_state);
void gpio_config_as_open_drain(gpio_t* gpio, 
                               gpio_status_e initial_state, 
                               gpio_pull_e pull);
void gpio_config_as_open_source(gpio_t* gpio, 
                                gpio_status_e initial_state, 
                                gpio_pull_e pull);
void gpio_add_trigger(gpio_t* gpio, 
                      gpio_trigger_e trigger, 
                      gpio_trigger_handler handler,
                      void* user_data);  
void gpio_disconnect(gpio_t* gpio);



void gpio_set(gpio_t* gpio);
void gpio_clear(gpio_t* gpio);
gpio_status_e gpio_read(gpio_t* gpio);




#endif /* GPIO_H */
