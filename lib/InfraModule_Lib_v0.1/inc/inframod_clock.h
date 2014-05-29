/*******************************************************************************
 * inframod_clock.h
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
#ifndef __INFRAMOD_CLOCK_H
#define __INFRAMOD_CLOCK_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_rtc.h"
#include "stm32l1xx_pwr.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_syscfg.h"
#include "misc.h"


/* Exported functions --------------------------------------------------------*/

void CLOCK_Init (void);
void CLOCK_EXTI_Init (void);
void CLOCK_Config (uint16_t time);


#endif  /*__INFRAMOD_CLOCK_H*/

/***************************************************************END OF FILE****/
