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

#include "stm32l4xx_ll_utils.h"



/*****************************************************************************/
/* MAIN FUNCTION */
/*****************************************************************************/

int main(void)
{
    system_init();
    led_init(LED_PIN_BOARD_GREEN_LD3);

    while (1) {
        led_toggle(LED_PIN_BOARD_GREEN_LD3);
        LL_mDelay(250u);
    }

    return 0;
}

