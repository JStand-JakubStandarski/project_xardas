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

#include "led.h"
#include "system.h"

#include "FreeRTOS.h"
#include "task.h"

#include "stm32l4xx_ll_utils.h"



/*****************************************************************************/
/* PRIVATE FUNCTIONS DECLARATIONS */
/*****************************************************************************/

static void blink_led(void *parameter);



/*****************************************************************************/
/* MAIN FUNCTION */
/*****************************************************************************/

int main(void)
{
    system_init();
    led_init(LED_PIN_BOARD_GREEN_LD3);

    BaseType_t return_code = xTaskCreate(blink_led, "blink_led",
        configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    if (return_code == pdPASS) {
        vTaskStartScheduler();
    } else {
        ;
    }

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

