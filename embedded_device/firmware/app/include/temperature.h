/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 03.02.2022
 */

#ifndef THERMOMETER_H
    #define THERMOMETER_H

/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

/**
 * @brief   Initialize thermometer.
 *
 * @param   None.
 *
 * @retval  None.
 */
void thermometer_init(void);



/**
 * @brief   Get temperature measured by thermometer.
 *
 * @param   None.
 *
 * @retval  Current temperature in Celsius.
 */
float thermometer_get_temperature(void);



#endif /* THERMOMETER_H */

