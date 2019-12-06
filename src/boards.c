/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Ha Thach for Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "boards.h"

#include "app_scheduler.h"
#include "app_timer.h"

//--------------------------------------------------------------------+
// MACRO TYPEDEF CONSTANT ENUM DECLARATION
//--------------------------------------------------------------------+
#define SCHED_MAX_EVENT_DATA_SIZE           sizeof(app_timer_event_t)        /**< Maximum size of scheduler events. */
#define SCHED_QUEUE_SIZE                    30                               /**< Maximum number of events in the scheduler queue. */

//------------- IMPLEMENTATION -------------//
void boards_init(void)
{
    // stop LF clock just in case we jump from application without reset
    NRF_CLOCK->TASKS_LFCLKSTOP = 1UL;

    // Use Internal OSC to compatible with all boards
    NRF_CLOCK->LFCLKSRC = CLOCK_LFCLKSRC_SRC_RC;
    NRF_CLOCK->TASKS_LFCLKSTART = 1UL;

    board_init();
    board_buttons_init();
    board_led_init();

    // Init scheduler
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

    // Init app timer (use RTC1)
    app_timer_init();

    // Configure Systick for led blinky
    NVIC_SetPriority(SysTick_IRQn, 7);
    SysTick_Config(SystemCoreClock/1000);
}

void boards_teardown(void)
{
    // Disable systick, turn off LEDs
    SysTick->CTRL = 0;

    // Disable and reset PWM for LEDs
    board_led_teardown();

    // Button
    board_button_teardown();

    board_teardown();

    // Stop RTC1 used by app_timer
    NVIC_DisableIRQ(RTC1_IRQn);
    NRF_RTC1->EVTENCLR    = RTC_EVTEN_COMPARE0_Msk;
    NRF_RTC1->INTENCLR    = RTC_INTENSET_COMPARE0_Msk;
    NRF_RTC1->TASKS_STOP  = 1;
    NRF_RTC1->TASKS_CLEAR = 1;

    // Stop LF clock
    NRF_CLOCK->TASKS_LFCLKSTOP = 1UL;
}


static uint32_t _systick_count = 0;
void SysTick_Handler(void)
{
    _systick_count++;
    board_led_tick();
}


uint32_t tusb_hal_millis(void)
{
  return ( ( ((uint64_t)app_timer_cnt_get())*1000*(APP_TIMER_CONFIG_RTC_FREQUENCY+1)) / APP_TIMER_CLOCK_FREQ );
}
