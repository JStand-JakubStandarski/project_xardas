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



