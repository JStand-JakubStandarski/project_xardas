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



void circular_buffer_reset(circular_buffer_t circular_buffer)
{
    if ((circular_buffer != NULL) && circular_buffer->assigned) {
        memset(circular_buffer->data_buffer, 0, DATA_BUFFER_SIZE_MAX);
        circular_buffer->data_count = 0;
        circular_buffer->head = 0;
        circular_buffer->tail = 0;
    }
}



bool circular_buffer_is_full(const circular_buffer_t circular_buffer)
{
    bool is_full = false;

    if ((circular_buffer != NULL) && circular_buffer->assigned) {
        is_full = (bool)(circular_buffer->data_count == DATA_BUFFER_SIZE_MAX);
    }

    return is_full;
}



bool circular_buffer_is_empty(const circular_buffer_t circular_buffer)
{
    bool is_empty = false;

    if ((circular_buffer != NULL) && circular_buffer->assigned) {
        is_empty = (bool)(circular_buffer->data_count == 0);
    }

    return is_empty;
}



size_t circular_buffer_get_free_space_size(
    const circular_buffer_t circular_buffer)
{
    size_t free_space_size = 0;

    if ((circular_buffer != NULL) && circular_buffer->assigned) {
        free_space_size = DATA_BUFFER_SIZE_MAX - circular_buffer->data_count;
    }

    return free_space_size;
}



void circular_buffer_write_data(circular_buffer_t circular_buffer,
    const uint8_t data)
{
    if ((circular_buffer != NULL) && circular_buffer->assigned) {
        if (circular_buffer->data_count < DATA_BUFFER_SIZE_MAX) {

            if (circular_buffer->head == DATA_BUFFER_SIZE_MAX) {
                circular_buffer->head = 0;
            }

            circular_buffer->data_buffer[circular_buffer->head] = data;
            ++circular_buffer->data_count;
            ++circular_buffer->head;
        }
    }
}



