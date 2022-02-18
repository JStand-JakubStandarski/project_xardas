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

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_gpio.h"
#include "stm32l4xx_ll_usart.h"

#include "stm32l432xx.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
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

#define DEBUG_MESSAGE_COUNT_MAX 10
#define DEBUG_MESSAGE_DATA_SIZE 64



/*****************************************************************************/
/* PRIVATE STRUCTURES */
/*****************************************************************************/

struct debug_message {
    char data[DEBUG_MESSAGE_DATA_SIZE];
    size_t size;
};



/*****************************************************************************/
/* PRIVATE VARIABLES */
/*****************************************************************************/

static TaskHandle_t debug_task_handle = NULL;
static QueueHandle_t debug_task_queue = NULL;
static QueueHandle_t uart_isr_queue = NULL;



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void debug_uart_gpio_init(void);
static void debug_uart_peripheral_init(void);



/*****************************************************************************/
/* RTOS TASK */
/*****************************************************************************/

static void debug_task(void *parameters)
{
    while (1) {

        struct debug_message debug_message = {0};
        BaseType_t new_message_received = xQueueReceive(debug_task_queue,
            (void *)&debug_message, portMAX_DELAY);
        if (new_message_received) {

            bool uart_bus_free = (bool)xTaskNotifyWait(0, 0, NULL,
                portMAX_DELAY);
            if (uart_bus_free) {

                BaseType_t message_sent_to_isr = xQueueSend(
                    uart_isr_queue, (void *)&debug_message, portMAX_DELAY);
                if (message_sent_to_isr) {

                    LL_USART_EnableIT_TXE(DEBUG_UART_PERIPHERAL_PORT);
                }
            }
        }
    }
}



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

void debug_init(void)
{
    debug_uart_gpio_init();
    debug_uart_peripheral_init();

    debug_task_queue = xQueueCreate(DEBUG_MESSAGE_COUNT_MAX,
        sizeof(struct debug_message));
    uart_isr_queue = xQueueCreate(DEBUG_MESSAGE_COUNT_MAX,
        sizeof(struct debug_message));
    if ((debug_task_queue != NULL) && (uart_isr_queue != NULL)) {

        xTaskCreate(debug_task, "debug_task", configMINIMAL_STACK_SIZE, NULL,
            1, &debug_task_handle);

        bool uart_bus_free = !(bool)LL_USART_IsEnabledIT_TXE(
            DEBUG_UART_PERIPHERAL_PORT);
        if (uart_bus_free) {
            xTaskNotify(debug_task_handle, 0, eNoAction);
        }
    }
}



void debug_printf(const char *text, ...)
{
    if (debug_task_queue != NULL) {

        struct debug_message debug_message = {0};

        va_list arguments;
        va_start(arguments, text);
        vsnprintf(debug_message.data, DEBUG_MESSAGE_DATA_SIZE, text,
            arguments);
        va_end(arguments);

        debug_message.size = strlen(debug_message.data);

        xQueueSend(debug_task_queue, (void *)&debug_message, 0);
    }
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

    NVIC_SetPriority(DEBUG_UART_PERIPHERAL_IRQ, 44);
    NVIC_EnableIRQ(DEBUG_UART_PERIPHERAL_IRQ);
    LL_USART_Enable(DEBUG_UART_PERIPHERAL_PORT);
}



/*****************************************************************************/
/* IRQ HANDLERS DEFINITIONS */
/*****************************************************************************/

void USART1_IRQHandler(void)
{
    static struct debug_message debug_message = {0};
    static size_t character_index = 0;

    if (LL_USART_IsActiveFlag_TXE(DEBUG_UART_PERIPHERAL_PORT) == 1) {

        static bool transfer_ongoing = false;
        static bool request_new_message = true;
        if (request_new_message) {

            BaseType_t new_message_ready = xQueueReceiveFromISR(uart_isr_queue,
                &debug_message, NULL);
            if (new_message_ready) {

                request_new_message = false;
                transfer_ongoing = true;
                character_index = 0;
            }
        }

        if (transfer_ongoing) {

            if (debug_message.size > 0) {

                LL_USART_TransmitData8(DEBUG_UART_PERIPHERAL_PORT,
                    debug_message.data[character_index]);
                ++character_index;
                --debug_message.size;

            }

            if (debug_message.size == 0) {

                transfer_ongoing = false;
                request_new_message = true;

                LL_USART_DisableIT_TXE(DEBUG_UART_PERIPHERAL_PORT);

                xTaskNotifyFromISR(debug_task_handle, 0, eNoAction, NULL);
            }
        }
    }
}

