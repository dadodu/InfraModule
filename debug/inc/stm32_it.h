/*******************************************************************************
 * stm32_it.h
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



/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_IT_H
#define __STM32_IT_H

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"


/* Exported functions ------------------------------------------------------- */

/* Interrupts ON/OFF button */
void EXTI0_IRQHandler (void);


#endif /*__STM32_IT_H*/

/***************************************************************END OF FILE****/

