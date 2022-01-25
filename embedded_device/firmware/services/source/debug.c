/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 21.01.2022
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "debug.h"
#include "circular_buffer.h"

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"

#include "stm32l432xx.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/*****************************************************************************/
/* PRIVATE MACROS */
/*****************************************************************************/

#define DEBUG_UART_GPIO_PORT                    GPIOA
#define DEBUG_UART_GPIO_CLOCK                   LL_AHB2_GRP1_PERIPH_GPIOA
#define DEBUG_UART_GPIO_PIN_TX                  LL_GPIO_PIN_9
#define DEBUG_UART_GPIO_PIN_ALTERNATE_FUNCTION  LL_GPIO_AF_7

#define DEBUG_UART_PERIPHERAL_PORT      USART1
#define DEBUG_UART_PERIPHERAL_IRQ       USART1_IRQn
#define DEBUG_UART_PERIPHERAL_CLOCK     LL_APB2_GRP1_PERIPH_USART1
#define DEBUG_UART_PERIPHERAL_BAUDRATE  115200



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void debug_uart_gpio_init(void);



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void debug_uart_gpio_init(void)
{
    if (LL_AHB2_GRP1_IsEnabledClock(DEBUG_UART_GPIO_CLOCK) == 0) {
        LL_AHB2_GRP1_EnableClock(DEBUG_UART_GPIO_CLOCK);
    }

    LL_GPIO_InitTypeDef uart_gpio_config = {
        .Pin = DEBUG_UART_GPIO_PIN_TX,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_LOW,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
        .Alternate = DEBUG_UART_GPIO_PIN_ALTERNATE_FUNCTION
    };
    LL_GPIO_Init(DEBUG_UART_GPIO_PORT, &uart_gpio_config);
}



