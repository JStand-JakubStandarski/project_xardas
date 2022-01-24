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



