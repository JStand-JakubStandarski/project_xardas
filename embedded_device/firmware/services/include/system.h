/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 06.03.2021
 *
 */

#ifndef SYSTEM_H
    #define SYSTEM_H

/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

/**
 * @brief   Initialize system (i.e. flash latency, system clock source,
 *          system clock speed, AHB1 prescaler, APB1 prescaler, APB2
 *          prescaler).
 *
 * @param   None.
 *
 * @retval  None.
 */
void system_init(void);



#endif /* SYSTEM_H */

