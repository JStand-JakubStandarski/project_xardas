/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 21.01.2022
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "circular_buffer.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>



/*****************************************************************************/
/* PRIVATE MACROS */
/*****************************************************************************/

#define CIRCULAR_BUFFER_AVAILABLE_COUNT ((size_t)4)

#define DATA_BUFFER_SIZE_MAX    ((size_t)256)



/*****************************************************************************/
/* PRIVATE STRUCTURES */
/*****************************************************************************/

struct circular_buffer {
    bool assigned;
    uint8_t data_buffer[DATA_BUFFER_SIZE_MAX];
    size_t data_count;
    size_t head;
    size_t tail;
};

static struct circular_buffer circular_buffers[CIRCULAR_BUFFER_AVAILABLE_COUNT];



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

circular_buffer_t circular_buffer_init(void)
{
    circular_buffer_t circular_buffer = NULL;

    for (size_t buffer_index = 0; buffer_index <
        CIRCULAR_BUFFER_AVAILABLE_COUNT; ++buffer_index) {
            if (circular_buffers[buffer_index].assigned == false) {
                circular_buffer = &circular_buffers[buffer_index];
                memset(circular_buffer->data_buffer, 0, DATA_BUFFER_SIZE_MAX);
                circular_buffer->data_count = 0;
                circular_buffer->head = 0;
                circular_buffer->tail = 0;
                circular_buffer->assigned = true;

                break;
            }
    }

    return circular_buffer;
}



void circular_buffer_close(circular_buffer_t circular_buffer)
{
    if ((circular_buffer != NULL) && circular_buffer->assigned) {
        memset(circular_buffer->data_buffer, 0, DATA_BUFFER_SIZE_MAX);
        circular_buffer->data_count = 0;
        circular_buffer->head = 0;
        circular_buffer->tail = 0;
        circular_buffer->assigned = false;
    }
}



