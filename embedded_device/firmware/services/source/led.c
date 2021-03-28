/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 07.03.2021
 *
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "led.h"

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"

#include <stdbool.h>



/*****************************************************************************/
/* PRIVATE MACROS */
/*****************************************************************************/

#define LED_PIN_BOARD_GREEN_LD3_GPIO_PORT   GPIOB
#define LED_PIN_BOARD_GREEN_LD3_GPIO_PIN    LL_GPIO_PIN_3



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void led_pin_board_green_ld3_gpio_init(void);



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

void led_init(led_pin_t led_pin)
{
    switch (led_pin) {
        case LED_PIN_BOARD_GREEN_LD3:
            led_pin_board_green_ld3_gpio_init();
        break;
    }
}



void led_on(led_pin_t led_pin)
{
    switch (led_pin) {
        case LED_PIN_BOARD_GREEN_LD3:
            LL_GPIO_SetOutputPin(LED_PIN_BOARD_GREEN_LD3_GPIO_PORT,
                LED_PIN_BOARD_GREEN_LD3_GPIO_PIN);
        break;
    }
}



void led_off(led_pin_t led_pin)
{
    switch (led_pin) {
        case LED_PIN_BOARD_GREEN_LD3:
            LL_GPIO_ResetOutputPin(LED_PIN_BOARD_GREEN_LD3_GPIO_PORT,
                LED_PIN_BOARD_GREEN_LD3_GPIO_PIN);
        break;
    }
}



void led_toggle(led_pin_t led_pin)
{
    switch (led_pin) {
        case LED_PIN_BOARD_GREEN_LD3:
            LL_GPIO_TogglePin(LED_PIN_BOARD_GREEN_LD3_GPIO_PORT,
                LED_PIN_BOARD_GREEN_LD3_GPIO_PIN);
        break;
    }
}



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void led_pin_board_green_ld3_gpio_init(void)
{
    bool gpiob_clock_on = (bool)LL_AHB2_GRP1_IsEnabledClock(
        LL_AHB2_GRP1_PERIPH_GPIOB);
    if (gpiob_clock_on != true) {
        LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    }

    LL_GPIO_InitTypeDef gpio_pin_config = {
        .Pin = LED_PIN_BOARD_GREEN_LD3_GPIO_PIN,
        .Mode = LL_GPIO_MODE_OUTPUT,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
    };
    LL_GPIO_Init(LED_PIN_BOARD_GREEN_LD3_GPIO_PORT, &gpio_pin_config);
}

