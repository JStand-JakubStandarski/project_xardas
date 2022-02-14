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
static void adc_init(void);



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



static void adc_init(void)
{
    if (LL_AHB2_GRP1_IsEnabledClock(LL_AHB2_GRP1_PERIPH_ADC) != 1) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_ADC);
    }

    if (LL_ADC_IsEnabled(ADC1) != 1) {
        LL_ADC_CommonInitTypeDef adc_common_config = {
            .CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV4
        };
        LL_ADC_CommonInit(ADC1_COMMON, &adc_common_config);

        LL_ADC_DisableDeepPowerDown(ADC1);

        LL_ADC_EnableInternalRegulator(ADC1);
        while (LL_ADC_IsInternalRegulatorEnabled(ADC1) != 1) {
            ;
        }

        LL_ADC_InitTypeDef adc_config = {
            .Resolution = LL_ADC_RESOLUTION_12B,
            .DataAlignment = LL_ADC_DATA_ALIGN_RIGHT,
            .LowPowerMode = LL_ADC_LP_MODE_NONE
        };
        LL_ADC_Init(ADC1, &adc_config);

        LL_ADC_REG_InitTypeDef adc_regular_config = {
            .TriggerSource = LL_ADC_REG_TRIG_EXT_TIM2_TRGO,
            .SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE,
            .SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE,
            .ContinuousMode = LL_ADC_REG_CONV_SINGLE,
            .DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE,
            .Overrun = LL_ADC_REG_OVR_DATA_OVERWRITTEN
        };
        LL_ADC_REG_Init(ADC1, &adc_regular_config);

        LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1,
            LL_ADC_CHANNEL_6);
        LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_6,
            LL_ADC_SAMPLINGTIME_640CYCLES_5);
        LL_ADC_SetChannelSingleDiff(ADC1, LL_ADC_CHANNEL_6,
            LL_ADC_SINGLE_ENDED);

        LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_GRP_REGULAR_CONTINUED);
        LL_ADC_SetOverSamplingDiscont(ADC1, LL_ADC_OVS_REG_CONT);
        LL_ADC_ConfigOverSamplingRatioShift(ADC1, LL_ADC_OVS_RATIO_16,
            LL_ADC_OVS_SHIFT_RIGHT_4);
        LL_ADC_REG_SetTriggerEdge(ADC1, LL_ADC_REG_TRIG_EXT_RISING);

        LL_ADC_StartCalibration(ADC1, LL_ADC_SINGLE_ENDED);
        while (LL_ADC_IsCalibrationOnGoing(ADC1)) {
            ;
        }

        LL_ADC_Enable(ADC1);
        while (LL_ADC_IsActiveFlag_ADRDY(ADC1) != 1) {
            ;
        }

        NVIC_SetPriority(ADC1_IRQn, 25);
        NVIC_EnableIRQ(ADC1_IRQn);
        LL_ADC_EnableIT_EOC(ADC1);

        LL_ADC_REG_StartConversion(ADC1);
    }
}



