/*******************************************************************************
 * hw_config.h
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
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "stm32l1xx_conf.h"

/* Libraries InfraModule_v0.1 */
#include "inframod_board.h"
#include "inframod_pwr.h"
#include "inframod_spi_flash.h"
#include "inframod_clock.h"
#include "inframod_delay.h"
#include "inframod_uart.h"

#include "xprintf.h"

/* Exported defines ----------------------------------------------------------*/

#define BLACK           0x0000
#define WHITE           0xFFFF
#define RED             0xF800
#define GREEN           0x07E0
#define BLUE            0x001F
#define YELLOW          0xFFE0
#define CYAN            0x07FF
#define LIGHT_BLUE      0x1C9F
#define ORANGE          0xFD20

/* Exported functions ------------------------------------------------------- */

void Set_System (void);

void Led_Config (void);
void Slide_Config (void);

void Led_Red_ON (void);
void Led_Red_OFF (void);
void Led_Green_ON (void);
void Led_Green_OFF (void);


#endif  /*__HW_CONFIG_H*/

/***************************************************************END OF FILE****/
