/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 21.01.2022
 */

#ifndef DEBUG_H
    #define DEBUG_H

/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

/**
 * @brief   Initialize primitive debug functionality - printing custom
 *          messages via UART.
 *
 * @param   None.
 *
 * @retval  None.
 */
void debug_init(void);



/**
 * @brief   Print custom debug message.
 *
 * @param   text - custom text message.
 *
 * @param   ... - arguments for text message.
 *
 * @retval  None.
 */
void debug_printf(const char *const text, ...);



#endif /* DEBUG_H */

