/*******************************************************************************
 * inframod_spi_flash.h
 * 
 * Copyright 2013 Roman GAUCHI
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
 *******************************************************************************
 * 
 *         +------------------------------------------------------+
 *         |                   Pin assignment                     |
 *         +-----------------------+-----------------+------------+
 *         |  STM32 SPI Pins       |      FLASH      |    Pins    |
 *         +-----------------------+-----------------+------------+
 *         | FLASH_CS_PIN          | ChipSelect (S#) |  1         |
 *         | FLASH_SPI_MISO_PIN    | DataOut    (DQ1)|  2         |
 *         |                       | Write      (W#) |  3         |
 *         |                       | GND             |  4 (0 V)   |
 *         | FLASH_SPI_MOSI_PIN    | DataIn     (DQ0)|  5         |
 *         | FLASH_SPI_SCK_PIN     | Clock      (C)  |  6         |
 *         |                       | Hold       (H#) |  7         |
 *         |                       | VCC             |  8 (3.3 V) |
 *         +-----------------------+-----------------+------------+
 * 
 ******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INFRAMOD_SPI_FLASH_H
#define __INFRAMOD_SPI_FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#include "stm32l1xx_gpio.h"
#include "stm32l1xx_rcc.h"
#include "stm32l1xx_spi.h"

#include "inframod_board.h"


/* Exported constants --------------------------------------------------------*/

/* Flash registers commands */
#define FLASH_CMD_WRITE             0x02    /* Write to Memory instruction */
#define FLASH_CMD_WRSR              0x01    /* Write Status Register instruction */
#define FLASH_CMD_WREN              0x06    /* Write enable instruction */
#define FLASH_CMD_READ              0x03    /* Read from Memory instruction */
#define FLASH_CMD_RDSR              0x05    /* Read Status Register instruction  */
#define FLASH_CMD_RDLR              0xE8    /* Read Lock Register instruction  */
#define FLASH_CMD_RDID              0x9F    /* Read identification */
#define FLASH_CMD_SSE               0x20    /* Subsector Erase instruction */
#define FLASH_CMD_SE                0xD8    /* Sector Erase instruction */
#define FLASH_CMD_BE                0xC7    /* Bulk Erase instruction */
#define FLASH_WIP_FLAG              0x01    /* Write In Progress (WIP) flag */
#define FLASH_DUMMY_BYTE            0xA5

#define FLASH_SPI_PAGESIZE          0x100   /* 512 bytes */

#define FLASH_N25Q032_ID            0x20BA16

/* Flash parameters */
#define FLASH_PAGE_SIZE             0x0200  /* 512 bytes per page w/o Spare Area */
#define FLASH_BLOCK_SIZE            0x0008  /* 8x512 bytes pages per block */
#define FLASH_ZONE_SIZE             0x0400  /* 1024 Block per zone */
#define FLASH_SPARE_AREA_SIZE       0x0010  /* last 16 bytes as spare area */
#define FLASH_MAX_ZONE              0x0001  /* 1 zones of 1024 block */


/* Control I/O pins */
#define FLASH_CS_LOW()              GPIO_LOW(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)
#define FLASH_CS_HIGH()             GPIO_HIGH(FLASH_CS_GPIO_PORT, FLASH_CS_PIN)
#define FLASH_WR_LOW()              GPIO_LOW(FLASH_WR_GPIO_PORT, FLASH_WR_PIN)
#define FLASH_WR_HIGH()             GPIO_HIGH(FLASH_WR_GPIO_PORT, FLASH_WR_PIN)
#define FLASH_HOLD_LOW()            GPIO_LOW(FLASH_HOLD_GPIO_PORT, FLASH_HOLD_PIN)
#define FLASH_HOLD_HIGH()           GPIO_HIGH(FLASH_HOLD_GPIO_PORT, FLASH_HOLD_PIN)

/* Exported functions --------------------------------------------------------*/

/* High layer functions */
void FLASH_Init(void);
void FLASH_EraseSector(uint32_t SectorAddr);
void FLASH_EraseBulk(void);
void FLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void FLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
uint32_t FLASH_ReadID(void);
void FLASH_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */
uint8_t FLASH_ReadByte(void);
uint8_t FLASH_SendByte(uint8_t byte);
uint16_t FLASH_SendHalfWord(uint16_t HalfWord);
void FLASH_WriteEnable(void);
void FLASH_WaitForWriteEnd(void);

void FLASH_WriteStatusRegister (uint8_t value);
uint8_t FLASH_ReadStatusRegister (void);

#endif  /*__INFRAMOD_SPI_FLASH_H */ 

/***************************************************************END OF FILE****/
