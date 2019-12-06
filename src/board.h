#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "config.h"

typedef enum led_status_e
{
  LED_STATUS_BOOTLOADER_STARTED = 0,
  LED_STATUS_USB_MOUNTED,
  LED_STATUS_USB_UNMOUNTED,
  LED_STATUS_WRITING_STARTED,
  LED_STATUS_WRITING_FINISHED,
  LED_STATUS_BLE_CONNECTED,
  LED_STATUS_BLE_DISCONNECTED,
} led_status_e;

typedef enum button_e
{
    BUTTON_DFU,
    BUTTON_FRESET,
} button_e;

void board_init(void);
void board_teardown(void);

void board_buttons_init(void);
bool board_button_pressed(button_e button);
void board_button_teardown(void);

void board_led_init(void);
void board_led_status(led_status_e status);
void board_led_tick();
void board_led_teardown(void);


// Helper function
#define memclr(buffer, size)                memset(buffer, 0, size)
#define varclr(_var)                        memclr(_var, sizeof(*(_var)))
#define arrclr(_arr)                        memclr(_arr, sizeof(_arr))
#define arrcount(_arr)                      ( sizeof(_arr) / sizeof(_arr[0]) )


//--------------------------------------------------------------------+
// DEBUG
//--------------------------------------------------------------------+
//#define CFG_DEBUG

#ifdef CFG_DEBUG

#include <stdio.h>

#define PRINT_LOCATION()      printf("%s: %d:\n", __PRETTY_FUNCTION__, __LINE__)
#define PRINT_MESS(x)         printf("%s: %d: %s \n"   , __FUNCTION__, __LINE__, (char*)(x))
#define PRTNT_HEAP()          if (CFG_DEBUG == 3) printf("\n%s: %d: Heap free: %d\n", __FUNCTION__, __LINE__, util_heap_get_free_size())
#define PRINT_STR(x)          printf("%s: %d: " #x " = %s\n"   , __FUNCTION__, __LINE__, (char*)(x) )
#define PRINT_INT(x)          printf("%s: %d: " #x " = %ld\n"  , __FUNCTION__, __LINE__, (uint32_t) (x) )
#define PRINT_HEX(x)          printf("%s: %d: " #x " = 0x%X\n"  , __FUNCTION__, __LINE__, (uint32_t) (x) )

#define PRINT_BUFFER(buf, n) \
  do {\
    uint8_t const* p8 = (uint8_t const*) (buf);\
    printf(#buf ": ");\
    for(uint32_t i=0; i<(n); i++) printf("%x ", p8[i]);\
    printf("\n");\
  }while(0)

#define ADALOG(tag, ...) \
  do { \
    if ( tag ) printf("[%s] ", tag);\
    printf(__VA_ARGS__);\
    printf("\n");\
  }while(0)

#else

#define PRINT_LOCATION()
#define PRINT_MESS(x)
#define PRTNT_HEAP()
#define PRINT_STR(x)
#define PRINT_INT(x)
#define PRINT_HEX(x)
#define PRINT_BUFFER(buf, n)

#define ADALOG(...)

#endif

bool is_ota(void);

#endif