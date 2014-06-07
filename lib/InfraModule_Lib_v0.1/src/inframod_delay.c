/*******************************************************************************
 * inframod_delay.c
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
#include "inframod_delay.h"


/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
 * @brief  : Delay in microseconds.
 * @param  : delay: delay time.
 * @return : None.
 ******************************************************************************/
void delay_us(__IO uint32_t nCount)
{
    while(nCount --)
    {
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
        asm("nop");
    }
}

/*******************************************************************************
 * @brief  : Delay in milliseconds.
 * @param  : delay: delay time.
 * @return : None.
 ******************************************************************************/
void delay_ms(__IO uint32_t nCount)
{
    uint16_t i;
    while(nCount--)
    {
        for(i=0; i<1000;i++)
            delay_us(1);
    }
}

/***************************************************************END OF FILE****/
