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

static void tick_count_logger(void *parameter);



/*****************************************************************************/
/* MAIN FUNCTION */
/*****************************************************************************/

int main(void)
{
    system_init();
    debug_init();
    thermometer_init();
    led_init(LED_PIN_BOARD_GREEN_LD3);

    debug_printf("Hello from STM32L432xx!\n");

    xTaskCreate(blink_led, "blink_led", configMINIMAL_STACK_SIZE, NULL, 1,
        NULL);
    xTaskCreate(tick_count_logger, "tick_count_logger",
        configMINIMAL_STACK_SIZE, NULL, 1, NULL);

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



static void tick_count_logger(void *parameter)
{
    while (1) {
        TickType_t tick_count = xTaskGetTickCount();
        debug_printf("Tick count since scheduler has started: %u\n",
            tick_count);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

