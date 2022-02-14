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



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void gpio_init(void);



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void gpio_init(void)
{
    if (LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_GPIOA) != 1) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    }

    LL_GPIO_InitTypeDef gpio_config = {
        .Pin = LL_GPIO_PIN_1,
        .Mode = LL_GPIO_MODE_ANALOG,
        .Speed = LL_GPIO_SPEED_FREQ_LOW,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = LL_GPIO_AF_0
    };
    LL_GPIO_Init(GPIOA, &gpio_config);
}



