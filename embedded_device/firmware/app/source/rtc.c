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



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void enable_rtc_init_mode(void);

static void disable_rtc_init_mode(void);


static void set_time_in_rtc(const rtc_time_t rtc_time);

static void set_date_in_rtc(const rtc_date_t rtc_date);





/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void enable_rtc_init_mode(void)
{
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
}



static void disable_rtc_init_mode(void)
{
    LL_RTC_DisableInitMode(RTC);

    bool shadow_registers_bypass_off = !LL_RTC_IsShadowRegBypassEnabled(RTC);
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
}



static void set_time_in_rtc(const rtc_time_t rtc_time)
{
    const uint32_t hour_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_time.hour);
    const uint32_t minute_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_time.minute);
    const uint32_t second_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_time.second);

    LL_RTC_TIME_Config(RTC, LL_RTC_TIME_FORMAT_AM_OR_24, hour_bcd, minute_bcd,
        second_bcd);
}



static void set_date_in_rtc(const rtc_date_t rtc_date)
{
    const uint32_t year_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.year -
        RTC_YEAR_OFFSET);
    const uint32_t month_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.month);
    const uint32_t day_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.day);
    const uint32_t weekday_bcd = __LL_RTC_CONVERT_BIN2BCD(rtc_date.weekday);

    LL_RTC_DATE_Config(RTC, weekday_bcd, day_bcd, month_bcd, year_bcd);
}

