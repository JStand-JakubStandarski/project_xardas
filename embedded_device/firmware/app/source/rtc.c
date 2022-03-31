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



