/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 07.03.2021
 *
 */

#ifndef LED_H
    #define LED_H

/*****************************************************************************/
/* PUBLIC ENUMS */
/*****************************************************************************/

typedef enum led_pin {
    LED_PIN_BOARD_GREEN_LD3
}led_pin_t;



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

/**
 * @brief   Initialize LED pin.
 *
 * @param   led_pin - LED pin @ref led_pin_t.
 *
 * @retval  None.
 */
void led_init(led_pin_t led_pin);



/**
 * @brief   Turn on LED pin.
 *
 * @param   led_pin - LED pin @ref led_pin_t.
 *
 * @retval  None.
 */
void led_on(led_pin_t led_pin);



/**
 * @brief   Turn off LED pin.
 *
 * @param   led_pin - LED pin @ref led_pin_t.
 *
 * @retval  None.
 */
void led_off(led_pin_t led_pin);



/**
 * @brief   Toggle LED pin.
 *
 * @param   led_pin - LED pin @ref led_pin_t.
 *
 * @retval  None.
 */
void led_toggle(led_pin_t led_pin);



#endif /* LED_H */

