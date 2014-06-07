/*******************************************************************************
 * inframod_spi_oled.h
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
#ifndef __INFRAMOD_SPI_OLED_H
#define __INFRAMOD_SPI_OLED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_spi.h"

#include "inframod_board.h"


/* Exported constants --------------------------------------------------------*/

#define OLED_CS_LOW()           GPIO_LOW(OLED_CS_PORT, OLED_CS_PIN)
#define OLED_CS_HIGH()          GPIO_HIGH(OLED_CS_PORT, OLED_CS_PIN)
#define OLED_RS_LOW()           GPIO_LOW(OLED_RS_PORT, OLED_RS_PIN)
#define OLED_RS_HIGH()          GPIO_HIGH(OLED_RS_PORT, OLED_RS_PIN)
#define OLED_RESET_LOW()        GPIO_LOW(OLED_RESET_PORT, OLED_RESET_PIN)
#define OLED_RESET_HIGH()       GPIO_HIGH(OLED_RESET_PORT, OLED_RESET_PIN)

#define OLED_VDDH_ON()          GPIO_HIGH(OLED_VDDH_PORT, OLED_VDDH_PIN)
#define OLED_VDDH_OFF()         GPIO_LOW(OLED_VDDH_PORT, OLED_VDDH_PIN)


/* Exported functions ------------------------------------------------------- */

void OLED_Init (void);
void OLED_VDDH_Init (void);

uint8_t OLED_SPI_SendByte (uint8_t byte);

#endif  /*__INFRAMOD_SPI_OLED_H*/

/***************************************************************END OF FILE****/
