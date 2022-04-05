/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 28.03.2021
 *
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "rtc.h"
#include "thermometer.h"

#include "debug.h"
#include "led.h"
#include "system.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32l4xx_ll_utils.h"



/*****************************************************************************/
/* PRIVATE FUNCTIONS DECLARATIONS */
/*****************************************************************************/

static void blink_led(void *parameter);

static void data_logger(void *parameter);

static void rtc_test_task(void *parameter);



/*****************************************************************************/
/* MAIN FUNCTION */
/*****************************************************************************/

int main(void)
{
    system_init();
    rtc_init();
    debug_init();
    thermometer_init();
    led_init(LED_PIN_BOARD_GREEN_LD3);

    debug_printf("Hello from STM32L432xx!\r\n");

    xTaskCreate(blink_led, "blink_led", configMINIMAL_STACK_SIZE, NULL, 1,
        NULL);
    xTaskCreate(data_logger, "data_logger", configMINIMAL_STACK_SIZE, NULL, 1,
        NULL);
    xTaskCreate(rtc_test_task, "rtc_test_task", configMINIMAL_STACK_SIZE, NULL,
        1, NULL);

    vTaskStartScheduler();

    while (1) {
        ;
    }

    return 0;
}



/*****************************************************************************/
/* PRIVATE FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void blink_led(void *parameter)
{
    while (1) {
        led_toggle(LED_PIN_BOARD_GREEN_LD3);
        vTaskDelay(250 / portTICK_PERIOD_MS);
    }
}



static void data_logger(void *parameter)
{
    while (1) {

        const TickType_t tick_count = xTaskGetTickCount();
        debug_printf("[SYSTEM] Time: %u [ticks]\r\n", tick_count);

        const float thermometer_temperature = thermometer_get_temperature();
        debug_printf("[THERMOMETER] Current temperature: %.2f [*C]\r\n",
            (double)thermometer_temperature);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}



static void rtc_test_task(void *parameter)
{
    const unsigned int function_call_delay_ms = 5000;

    const rtc_time_t default_rtc_time = {
        .hour = 23,
        .minute = 59,
        .second = 50
    };
    rtc_set_time(default_rtc_time, function_call_delay_ms);

    const rtc_date_t default_rtc_date = {
        .year = 2022,
        .month = RTC_MONTH_DECEMBER,
        .day = 31,
        .weekday = RTC_WEEKDAY_SATURDAY
    };
    rtc_set_date(default_rtc_date, function_call_delay_ms);

    while (1) {

        const rtc_time_t rtc_time = rtc_get_time(function_call_delay_ms);
        debug_printf("[RTC] Time: %02u:%02u:%02u\r\n", rtc_time.hour,
            rtc_time.minute, rtc_time.second);
        if ((rtc_time.hour == 0) && (rtc_time.minute == 0) &&
            (rtc_time.second == 30)) {

            rtc_set_time(default_rtc_time, function_call_delay_ms);
        }

        const rtc_date_t rtc_date = rtc_get_date(function_call_delay_ms);
        debug_printf("[RTC] Date: %02u-%02u-%04u [Weekday: %u]\r\n",
            rtc_date.day, rtc_date.month, rtc_date.year, rtc_date.weekday);
        if (rtc_date.day == 2) {

            rtc_set_date(default_rtc_date, function_call_delay_ms);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
