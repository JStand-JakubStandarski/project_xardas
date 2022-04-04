/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 06.03.2021
 *
 */

/*****************************************************************************/
/* HEADER FILES */
/*****************************************************************************/

#include "generic.h"
#include "system.h"

#include "stm32l4xx_ll_rcc.h"
#include "stm32l4xx_ll_bus.h"
#include "stm32l4xx_ll_pwr.h"
#include "stm32l4xx_ll_system.h"
#include "stm32l4xx_ll_utils.h"



/*****************************************************************************/
/* PRIVATE VARIABLES */
/*****************************************************************************/

static const uint32_t system_clock_speed_hz = 80000000u;



/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS PROTOTYPES */
/*****************************************************************************/

static void get_access_to_backup_domain_control_register(void);

static void config_pwr(void);

static void config_lse(void);



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

void system_init(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4) {
        ;
    }

    LL_RCC_MSI_Enable();
    while (LL_RCC_MSI_IsReady() != FLAG_STATE_SET) {
        ;
    }

    LL_RCC_MSI_EnableRangeSelection();
    while (LL_RCC_MSI_IsEnabledRangeSelect() != FLAG_STATE_SET) {
        ;
    }
    LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_6);

    LL_RCC_PLL_Disable();
    while (LL_RCC_PLL_IsReady() != FLAG_STATE_RESET) {
        ;
    }
    
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1,
        (uint32_t)40, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_EnableDomain_SYS();

    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != FLAG_STATE_SET) {
        ;
    }

    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
        ;
    }
    LL_SetSystemCoreClock(system_clock_speed_hz);

    /* Note: It has to be commented out while using FreeRTOS.
     *
     * LL_Init1msTick(system_clock_speed_hz);
     */

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
/*****************************************************************************/
/* PRIVATE HELPER FUNCTIONS DEFINITIONS */
/*****************************************************************************/

static void get_access_to_backup_domain_control_register(void)
{
    LL_PWR_EnableBkUpAccess();

    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
}



static void config_pwr(void)
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
}



static void config_lse(void)
{
    LL_RCC_LSE_SetDriveCapability(LL_RCC_LSEDRIVE_LOW);

    LL_RCC_LSE_Enable();
    while (LL_RCC_LSE_IsReady() != 1) {
        ;
    }
}



}

