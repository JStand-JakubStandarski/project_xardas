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



