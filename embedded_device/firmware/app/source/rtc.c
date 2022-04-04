/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 08.03.2022
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "debug.h"
#include "rtc.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "queue.h"

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_rtc.h"

#include "stm32l432xx.h"

#include <stdbool.h>
#include <time.h>



/*****************************************************************************/
/* PRIVATE MACROS */
/*****************************************************************************/

#define RTC_YEAR_OFFSET 2000



/*****************************************************************************/
/* PRIVATE VARIABLES */
/*****************************************************************************/

static TaskHandle_t rtc_task_handle = NULL;

static QueueHandle_t rtc_time_queue = NULL;

static QueueHandle_t rtc_date_queue = NULL;

static SemaphoreHandle_t rtc_registers_mutex = NULL;



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void enable_rtc_init_mode(void);

static void disable_rtc_init_mode(void);


static void set_time_in_rtc(const rtc_time_t rtc_time);

static void set_date_in_rtc(const rtc_date_t rtc_date);



/*****************************************************************************/
/* RTOS TASK */
/*****************************************************************************/

static void rtc_task(void *parameters)
{
    while (1) {

        static bool rtc_init_mode_on = false;

        const UBaseType_t new_rtc_time_count = uxQueueMessagesWaiting(
            rtc_time_queue);
        const UBaseType_t new_rtc_date_count = uxQueueMessagesWaiting(
            rtc_date_queue);
        if ((new_rtc_time_count > 0) || (new_rtc_date_count > 0)) {

            if (!rtc_init_mode_on) {

                enable_rtc_init_mode();
                rtc_init_mode_on = true;
            }

            rtc_time_t rtc_time = {0};
            const BaseType_t new_rtc_time_received = xQueueReceive(
                rtc_time_queue, &rtc_time, 0);
            if (new_rtc_time_received) {

                set_time_in_rtc(rtc_time);
            }

            rtc_date_t rtc_date = {0};
            const BaseType_t new_rtc_date_received = xQueueReceive(
                rtc_date_queue, &rtc_date, 0);
            if (new_rtc_date_received) {

                set_date_in_rtc(rtc_date);
            }

        } else {

            if (rtc_init_mode_on) {

                disable_rtc_init_mode();
                rtc_init_mode_on = false;
            }

            vTaskSuspend(NULL);
        }
    }
}



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

void rtc_init(void)
{
    LL_RTC_InitTypeDef rtc_init_config = {
        .HourFormat = LL_RTC_HOURFORMAT_24HOUR,
        .AsynchPrescaler = 127,
        .SynchPrescaler = 255
    };
    LL_RTC_Init(RTC, &rtc_init_config);

    LL_RTC_EnableRefClock(RTC);

    xTaskCreate(rtc_task, "rtc_task", configMINIMAL_STACK_SIZE, NULL, 2,
        &rtc_task_handle);

    rtc_time_queue = xQueueCreate(5, sizeof(rtc_time_t));

    rtc_date_queue = xQueueCreate(5, sizeof(rtc_date_t));

    rtc_registers_mutex = xSemaphoreCreateMutex();
}



rtc_time_t rtc_get_time(const unsigned int function_call_delay_ms)
{
    uint32_t time_bcd = 0;

    const BaseType_t mutex_taken = xSemaphoreTake(rtc_registers_mutex,
        pdMS_TO_TICKS(function_call_delay_ms));
    if (mutex_taken) {

        time_bcd = LL_RTC_TIME_Get(RTC);

        xSemaphoreGive(rtc_registers_mutex);
    }

    const rtc_time_t current_time = {
        .hour = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_HOUR(time_bcd)),
        .minute = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_MINUTE(time_bcd)),
        .second = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_SECOND(time_bcd))
    };

    return current_time;
}



void rtc_set_time(const rtc_time_t rtc_time,
    const unsigned int function_call_delay_ms)
{
    const BaseType_t item_sent = xQueueSend(rtc_time_queue, &rtc_time,
        pdMS_TO_TICKS(function_call_delay_ms));
    if (item_sent) {

        vTaskResume(rtc_task_handle);
    }
}



rtc_date_t rtc_get_date(const unsigned int function_call_delay_ms)
{
    uint32_t date_bcd = 0;

    const BaseType_t mutex_taken = xSemaphoreTake(rtc_registers_mutex,
        pdMS_TO_TICKS(function_call_delay_ms));
    if (mutex_taken) {

        date_bcd = LL_RTC_DATE_Get(RTC);

        xSemaphoreGive(rtc_registers_mutex);
    }

    const rtc_date_t current_date = {
        .year = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_YEAR(date_bcd)) +
            RTC_YEAR_OFFSET,
        .month = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_MONTH(date_bcd)),
        .day = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_DAY(date_bcd)),
        .weekday = __LL_RTC_CONVERT_BCD2BIN(__LL_RTC_GET_WEEKDAY(date_bcd))
    };

    return current_date;
}



void rtc_set_date(const rtc_date_t rtc_date,
    const unsigned int function_call_delay_ms)
{
    const BaseType_t item_sent = xQueueSend(rtc_date_queue, &rtc_date,
        pdMS_TO_TICKS(function_call_delay_ms));
    if (item_sent) {

        vTaskResume(rtc_task_handle);
    }
}



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void enable_rtc_init_mode(void)
{
    const BaseType_t mutex_taken = xSemaphoreTake(rtc_registers_mutex,
        portMAX_DELAY);
    if (mutex_taken) {

        LL_RTC_DisableWriteProtection(RTC);

        bool rtc_init_mode_off = !LL_RTC_IsActiveFlag_INIT(RTC);
        if (rtc_init_mode_off) {

            LL_RTC_EnableInitMode(RTC);

            rtc_init_mode_off = !LL_RTC_IsActiveFlag_INIT(RTC);
            while (rtc_init_mode_off) {

                vTaskDelay(pdMS_TO_TICKS(1));
                rtc_init_mode_off = !LL_RTC_IsActiveFlag_INIT(RTC);
            }
        }

        xSemaphoreGive(rtc_registers_mutex);
    }
}



static void disable_rtc_init_mode(void)
{
    const BaseType_t mutex_taken = xSemaphoreTake(rtc_registers_mutex,
        portMAX_DELAY);
    if (mutex_taken) {
        LL_RTC_DisableInitMode(RTC);

        bool shadow_registers_bypass_off = !LL_RTC_IsShadowRegBypassEnabled(
            RTC);
        if (shadow_registers_bypass_off) {

            LL_RTC_ClearFlag_RS(RTC);

            bool time_and_date_registers_desynchronized =
                !LL_RTC_IsActiveFlag_RS(RTC);
            while (time_and_date_registers_desynchronized) {

                vTaskDelay(pdMS_TO_TICKS(1));
                time_and_date_registers_desynchronized =
                    !LL_RTC_IsActiveFlag_RS(RTC);
            }
        }

        LL_RTC_EnableWriteProtection(RTC);

        xSemaphoreGive(rtc_registers_mutex);
    }
}



static void set_time_in_rtc(const rtc_time_t rtc_time)
{
    const uint32_t hour_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_time.hour);
    const uint32_t minute_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_time.minute);
    const uint32_t second_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_time.second);

    const BaseType_t mutex_taken = xSemaphoreTake(rtc_registers_mutex,
        portMAX_DELAY);
    if (mutex_taken) {

        LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24, hour_bcd,
            minute_bcd, second_bcd);

        xSemaphoreGive(rtc_registers_mutex);
    }
}



static void set_date_in_rtc(const rtc_date_t rtc_date)
{
    const uint32_t year_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.year -
        RTC_YEAR_OFFSET);
    const uint32_t month_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.month);
    const uint32_t day_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.day);
    const uint32_t weekday_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.weekday);

    const BaseType_t mutex_taken = xSemaphoreTake(rtc_registers_mutex,
        portMAX_DELAY);
    if (mutex_taken) {

        LL_RTC_DATE_Config(RTC, weekday_bcd, day_bcd, month_bcd, year_bcd);

        xSemaphoreGive(rtc_registers_mutex);
    }
}

