/*******************************************************************************
 * inframod_pwr.h
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
#ifndef __INFRAMOD_PWR_H
#define __INFRAMOD_PWR_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_i2c.h"
#include "stm32l1xx_exti.h"
#include "stm32l1xx_syscfg.h"
#include "misc.h"

#include "inframod_board.h"

/* Exported constants --------------------------------------------------------*/

#define Timed(x) timeout=0xFFFF; error++; while (x) { if (timeout-- == 0) return error;}

#define PWR_I2C_SPEED               400000      // 400kHz

#define PWR_ADDR_ID                 0x60
#define PWR_BUCK2_ADDR              0x06
/*****************************************
 * BUCK2 Output Voltage : 1.8V   => 0x41 *
 *                        2.8V   => 0x4B *
 *                        3.3V   => 0x50 *
 *                        RESET  => 0x5F *
 *****************************************/
#define PWR_BUCK2_VDD               0x4B            // 2.8V

/* Exported functions ------------------------------------------------------- */

void PWR_Init (void);
void PWR_EXTI_Init (void);

uint8_t PWR_WriteReg (uint8_t reg, uint8_t val);
uint8_t PWR_ReadReg (uint8_t reg, uint8_t *val);

#endif  /*__INFRAMOD_PWR_H*/

/***************************************************************END OF FILE****/
