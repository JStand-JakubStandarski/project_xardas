/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 21.01.2022
 */

#ifndef CIRCULAR_BUFFER_H
    #define CIRCULAR_BUFFER_H

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



/*****************************************************************************/
/* PUBLIC HANDLE */
/*****************************************************************************/

typedef struct circular_buffer *circular_buffer_t;



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

/**
 * @brief   Initialize circular buffer.
 *
 * @param   None.
 *
 * @retval  Handle to the circular buffer.
 */
circular_buffer_t circular_buffer_init(void);



/**
 * @brief   Return circular buffer to the available pool.
 *
 * @param   circular_buffer - handle to the circular buffer.
 *
 * @retval  None.
 */
void circular_buffer_close(circular_buffer_t circular_buffer);





#endif /* CIRCULAR_BUFFER_H */

