/*******************************************************************************
 * hw_config.c
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
#include "hw_config.h"


/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
/* !< At this stage the microcontroller clock setting is already configured,
 *    this is done through SystemInit() function which is called from startup
 *    file (startup_stm32xxx.s) before to branch to application main.
 *    To reconfigure the default setting of SystemInit() function, refer to
 *    system_stm32xxx.c file
 */
}

/*******************************************************************************
* Function Name  : Led_Config
* Description    : Configure the Read/Write LEDs.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Led_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // Enable Clocks
    RCC_AHBPeriphClockCmd(LED_GREEN_CLK | LED_RED_CLK, ENABLE);
    
    // Configure the green led
    GPIO_InitStructure.GPIO_Pin   = LED_GREEN_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(LED_GREEN_PORT, &GPIO_InitStructure);
    
    // Configure the red led
    GPIO_InitStructure.GPIO_Pin   = LED_RED_PIN;
    GPIO_Init(LED_RED_PORT, &GPIO_InitStructure);
    
    GPIO_LOW(LED_GREEN_PORT, LED_GREEN_PIN);
    GPIO_LOW(LED_RED_PORT, LED_RED_PIN);
}

/*******************************************************************************
* Function Name  : Slide_Config
* Description    : Configure the switch slide.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Slide_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // Enable Clocks
    RCC_AHBPeriphClockCmd(SLIDE_1_GPIO_CLK | SLIDE_2_GPIO_CLK, ENABLE);
    
    // Configure le Slide 1 en entree (pull-up) :
    GPIO_InitStruct.GPIO_Pin   = SLIDE_1_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(SLIDE_1_GPIO_PORT, &GPIO_InitStruct);
    
    // Configure le Slide 2 en entree :
    GPIO_InitStruct.GPIO_Pin   = SLIDE_2_PIN;
    GPIO_Init(SLIDE_2_GPIO_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
* Function Name  : Led_Red_ON
* Description    : Turn on the red light.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Led_Red_ON(void)
{
    GPIO_HIGH(LED_RED_PORT, LED_RED_PIN);
}

/*******************************************************************************
* Function Name  : Led_Red_OFF
* Description    : Turn off the red light.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Led_Red_OFF(void)
{
    GPIO_LOW(LED_RED_PORT, LED_RED_PIN);
}

/*******************************************************************************
* Function Name  : Led_Green_ON
* Description    : Turn on the green light.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Led_Green_ON(void)
{
    GPIO_HIGH(LED_GREEN_PORT, LED_GREEN_PIN);
}

/*******************************************************************************
* Function Name  : Led_Green_OFF
* Description    : Turn off the green light.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Led_Green_OFF(void)
{
    GPIO_LOW(LED_GREEN_PORT, LED_GREEN_PIN);
}

/***************************************************************END OF FILE****/
