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
/* PRIVATE OBJECTS & HANDLES */
/*****************************************************************************/

static circular_buffer_t debug_message_buffer = NULL;



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void debug_uart_gpio_init(void);

static void debug_uart_peripheral_init(void);



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

void debug_init(void)
{
    debug_uart_gpio_init();
    debug_uart_peripheral_init();

    debug_message_buffer = circular_buffer_init();
}



void debug_printf(const char *const text, ...)
{
    size_t output_buffer_size = circular_buffer_get_free_space_size(
        debug_message_buffer);
    char *formatted_message = calloc(output_buffer_size, sizeof(char));

    va_list arguments;
    va_start(arguments, text);
    vsnprintf(formatted_message, output_buffer_size, text, arguments);
    va_end(arguments);

    size_t formatted_message_size = strlen(formatted_message);
    if (formatted_message_size <= output_buffer_size) {
        for (size_t char_index = 0; char_index < formatted_message_size;
            ++char_index) {
                circular_buffer_write_data(debug_message_buffer,
                    formatted_message[char_index]);
        }
        LL_USART_EnableIT_TXE(DEBUG_UART_PERIPHERAL_PORT);
    };

    free(formatted_message);
}



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



static void debug_uart_peripheral_init(void)
{
    if (LL_APB2_GRP1_IsEnabledClock(DEBUG_UART_PERIPHERAL_CLOCK) == 0) {
        LL_APB2_GRP1_EnableClock(DEBUG_UART_PERIPHERAL_CLOCK);
    }

    LL_USART_InitTypeDef uart_peripheral_config = {
        .BaudRate = DEBUG_UART_PERIPHERAL_BAUDRATE,
        .DataWidth = LL_USART_DATAWIDTH_8B,
        .StopBits = LL_USART_STOPBITS_1,
        .Parity = LL_USART_PARITY_NONE,
        .TransferDirection = LL_USART_DIRECTION_TX,
        .HardwareFlowControl = LL_USART_HWCONTROL_NONE,
        .OverSampling = LL_USART_OVERSAMPLING_16
    };
    LL_USART_Init(DEBUG_UART_PERIPHERAL_PORT, &uart_peripheral_config);

    LL_USART_Enable(DEBUG_UART_PERIPHERAL_PORT);
    NVIC_EnableIRQ(DEBUG_UART_PERIPHERAL_IRQ);
}



