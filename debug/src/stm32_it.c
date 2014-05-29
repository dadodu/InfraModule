/*******************************************************************************
 * stm32_it.c
 * 
 * Copyright 2014 Roman GAUCHI
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 ******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stm32_it.h"


/******************************************************************************/
/*              Cortex-M Processor Exceptions Handlers                        */
/******************************************************************************/

/*******************************************************************************
* Function Name  : EXTI0_IRQHandler.
* Description    : Shutdown the system.
* Input          : None.
* Return         : None.
*******************************************************************************/
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        // Reset interrupt
        EXTI_ClearITPendingBit(EXTI_Line0);
        
        GPIO_HIGH(LED_RED_PORT, LED_RED_PIN);
        GPIO_HIGH(LED_GREEN_PORT, LED_GREEN_PIN);
        
        // Power down VCC_OLED
        //OLED_VDDH_OFF();
        delay_ms(100);
        
        // As you press the ON/OFF button
        while(GPIO_ReadInputDataBit(PWR_ONSTAT_GPIO_PORT, PWR_ONSTAT_PIN));
        
        // Power down VCC_NUM
        GPIO_LOW(PWR_POWERACK_GPIO_PORT, PWR_POWERACK_PIN);
    }
}

/*******************************************************************************
* Function Name  : RTC_WKUP_IRQHandler.
* Description    : Update the clock every second.
* Input          : None.
* Return         : None.
*******************************************************************************/
void RTC_WKUP_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line20);
        
        // blinking the green led
        GPIO_TOGGLE(LED_GREEN_PORT, LED_GREEN_PIN);
    }
}

/***************************************************************END OF FILE****/
