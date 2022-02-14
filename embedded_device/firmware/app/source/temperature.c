/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 03.02.2022
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "temperature.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

#include "stm32l4xx_ll_adc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_tim.h"

#include "stm32l432xx.h"

#include <limits.h>
#include <math.h>



/*****************************************************************************/
/* PRIVATE VARIABLES */
/*****************************************************************************/

static TaskHandle_t thermometer_task_handle = NULL;
static SemaphoreHandle_t thermometer_temperature_mutex = NULL;
static float thermometer_temperature = 0.0f;



