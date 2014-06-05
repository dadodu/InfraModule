/*******************************************************************************
 * inframod_spi_flash.c
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

/* Includes ------------------------------------------------------------------*/
#include "inframod_spi_flash.h"


/* Privates prototypes -------------------------------------------------------*/
void FLASH_LowLevel_Init (void);
void FLASH_HoldProtect_Init (void);
void FLASH_WriteProtect_Init (void);

/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
 * @brief  : Initializes the peripherals used by the SPI FLASH driver.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
    FLASH_HoldProtect_Init();
    FLASH_WriteProtect_Init();
    
    FLASH_LowLevel_Init();
    
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    /* SPI configuration */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(FLASH_SPI, &SPI_InitStructure);
    
    /* Enable the FLASH_SPI */
    SPI_Cmd(FLASH_SPI, ENABLE);
}

/*******************************************************************************
 * @brief  : Initializes the pins used by the SPI FLASH driver.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable clocks */
    RCC_AHBPeriphClockCmd(  FLASH_SPI_SCK_GPIO_CLK |
                            FLASH_SPI_MISO_GPIO_CLK | 
                            FLASH_SPI_MOSI_GPIO_CLK | 
                            FLASH_CS_GPIO_CLK, 
                            ENABLE);
    
    /* Enable the SPI clock */
    RCC_APB1PeriphClockCmd(FLASH_SPI_CLK, ENABLE);
    
    /* SCK pin */
    GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI_MISO pin */
    GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_MISO_PIN;
    GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI_MOSI pin */
    GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
    GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* Alternative functions */
    GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT, FLASH_SPI_SCK_SOURCE, FLASH_SPI_SCK_AF);
    GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT, FLASH_SPI_MISO_SOURCE, FLASH_SPI_MISO_AF);
    GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT, FLASH_SPI_MOSI_SOURCE, FLASH_SPI_MOSI_AF);
    
    /* CS pin */
    GPIO_InitStructure.GPIO_Pin   = FLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 * @brief  : Initializes the HOLD pin.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_HoldProtect_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the clock */
    RCC_AHBPeriphClockCmd(FLASH_HOLD_GPIO_CLK, ENABLE);
    
    /* HOLD pin */
    GPIO_InitStructure.GPIO_Pin   = FLASH_HOLD_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FLASH_HOLD_GPIO_PORT, &GPIO_InitStructure);
    
    /* Initializes the pin to high state */
    FLASH_HOLD_HIGH();
}

/*******************************************************************************
 * @brief  : Initializes the WR/Vpp pin.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_WriteProtect_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable the clock */
    RCC_AHBPeriphClockCmd(FLASH_WR_GPIO_CLK, ENABLE);
    
    /* WR pin */
    GPIO_InitStructure.GPIO_Pin   = FLASH_WR_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FLASH_HOLD_GPIO_PORT, &GPIO_InitStructure);
    
    /* Initializes the pin to high state */
    FLASH_WR_HIGH();
}

/*******************************************************************************
 * @brief  : Erases the specified Flash sector.
 * @param  : SectorAddr: address of the sector to erase.
 * @retval : None.
 ******************************************************************************/
void FLASH_EraseSector(uint32_t SectorAddr)
{
    /* Send write enable instruction */
    FLASH_WriteEnable();
    
    /* Sector Erase */
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send Sector Erase instruction */
    FLASH_SendByte(FLASH_CMD_SE);
    /* Send SectorAddr high nibble address byte */
    FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
    /* Send SectorAddr medium nibble address byte */
    FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    FLASH_SendByte(SectorAddr & 0xFF);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    /* Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * @brief  : Erases the entire Flash.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_EraseBulk(void)
{
    /* Send write enable instruction */
    FLASH_WriteEnable();
    
    /* Bulk Erase */
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send Bulk Erase instruction  */
    FLASH_SendByte(FLASH_CMD_BE);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    /* Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * @brief  : Writes more than one byte to the FLASH with a single WRITE cycle 
 *           (Page WRITE sequence).
 * @note   : The number of byte can't exceed the FLASH page size.
 * @param  : pBuffer: pointer to the buffer  containing the data to be written
 *           to the FLASH.
 *           WriteAddr: FLASH's internal address to write to.
 *           NumByteToWrite: number of bytes to write to the FLASH, must be 
 *           equal or less than "FLASH_PAGESIZE" value.
 * @retval : None.
 ******************************************************************************/
void FLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    FLASH_WriteEnable();
    
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send "Write to Memory " instruction */
    FLASH_SendByte(FLASH_CMD_WRITE);
    /* Send WriteAddr high nibble address byte to write to */
    FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
    /* Send WriteAddr medium nibble address byte to write to */
    FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
    /* Send WriteAddr low nibble address byte to write to */
    FLASH_SendByte(WriteAddr & 0xFF);
    
    /* while there is data to be written on the FLASH */
    while (NumByteToWrite--)
    {
        /* Send the current byte */
        FLASH_SendByte(*pBuffer);
        /* Point on the next byte to be written */
        pBuffer++;
    }
    
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    /* Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * @brief  : Writes block of data to the Flash. In this function, the number of
 *           WRITE cycles are reduced, using Page WRITE sequence.
 * @param  : pBuffer: pointer to the buffer  containing the data to be written
 *           to the Flash.
 *           WriteAddr: Flash's internal address to write to.
 *           NumByteToWrite: number of bytes to write to the Flash.
 * @retval : None.
 ******************************************************************************/
void FLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    
    Addr = WriteAddr % FLASH_SPI_PAGESIZE;
    count = FLASH_SPI_PAGESIZE - Addr;
    NumOfPage =  NumByteToWrite / FLASH_SPI_PAGESIZE;
    NumOfSingle = NumByteToWrite % FLASH_SPI_PAGESIZE;
    
    if (Addr == 0) /* WriteAddr is FLASH_PAGESIZE aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < FLASH_PAGESIZE */
        {
            FLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
        }
        else /* NumByteToWrite > FLASH_PAGESIZE */
        {
            while (NumOfPage--)
            {
                FLASH_WritePage(pBuffer, WriteAddr, FLASH_SPI_PAGESIZE);
                WriteAddr +=  FLASH_SPI_PAGESIZE;
                pBuffer += FLASH_SPI_PAGESIZE;
            }
            
            FLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else /* WriteAddr is not FLASH_PAGESIZE aligned */
    {
        if (NumOfPage == 0) /* NumByteToWrite < FLASH_PAGESIZE */
        {
            if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > FLASH_PAGESIZE */
            {
                temp = NumOfSingle - count;
                
                FLASH_WritePage(pBuffer, WriteAddr, count);
                WriteAddr +=  count;
                pBuffer += count;
                
                FLASH_WritePage(pBuffer, WriteAddr, temp);
            }
            else
            {
                FLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else /* NumByteToWrite > FLASH_PAGESIZE */
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / FLASH_SPI_PAGESIZE;
            NumOfSingle = NumByteToWrite % FLASH_SPI_PAGESIZE;
            
            FLASH_WritePage(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;
            
            while (NumOfPage--)
            {
                FLASH_WritePage(pBuffer, WriteAddr, FLASH_SPI_PAGESIZE);
                WriteAddr +=  FLASH_SPI_PAGESIZE;
                pBuffer += FLASH_SPI_PAGESIZE;
            }
            
            if (NumOfSingle != 0)
            {
                FLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/*******************************************************************************
 * @brief  : Reads a block of data from the Flash.
 * @param  : pBuffer: pointer to the buffer that receives the data read from 
 *           the Flash.
 *           ReadAddr: Flash's internal address to read from.
 *           NumByteToRead: number of bytes to read from the Flash.
 * @retval : None.
 ******************************************************************************/
void FLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    
    /* Send "Read from Memory " instruction */
    FLASH_SendByte(FLASH_CMD_READ);
    
    /* Send ReadAddr high nibble address byte to read from */
    FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte to read from */
    FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte to read from */
    FLASH_SendByte(ReadAddr & 0xFF);
    
    while (NumByteToRead--) /* while there is data to be read */
    {
        /* Read a byte from the FLASH */
        *pBuffer = FLASH_SendByte(FLASH_DUMMY_BYTE);
        /* Point to the next location where the byte read will be saved */
        pBuffer++;
    }
    
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Reads Flash identification.
 * @param  : None.
 * @retval : Flash identification.
 ******************************************************************************/
uint32_t FLASH_ReadID(void)
{
    uint8_t Temp0 = 0, Temp1 = 0, Temp2 = 0;
    uint32_t Temp = 0;
    
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send "RDID " instruction */
    FLASH_SendByte(FLASH_CMD_RDID);
    /* Read a byte from the FLASH */
    Temp0 = FLASH_SendByte(FLASH_DUMMY_BYTE);
    /* Read a byte from the FLASH */
    Temp1 = FLASH_SendByte(FLASH_DUMMY_BYTE);
    /* Read a byte from the FLASH */
    Temp2 = FLASH_SendByte(FLASH_DUMMY_BYTE);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
    return Temp;
}

/*******************************************************************************
 * @brief  : Initiates a read data byte (READ) sequence from the Flash.
 *           This is done by driving the /CS line low to select the device, 
 *           then the READ instruction is transmitted followed by 3 bytes 
 *           address. This function exit and keep the /CS line low, so the 
 *           Flash still being selected. With this technique the whole content 
 *           of the Flash is read with a single READ instruction.
 * @param  : ReadAddr: FLASH's internal address to read from.
 * @retval : None.
 ******************************************************************************/
void FLASH_StartReadSequence(uint32_t ReadAddr)
{
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    
    /* Send "Read from Memory " instruction */
    FLASH_SendByte(FLASH_CMD_READ);
    
    /* Send the 24-bit address of the address to read from -------------------*/
    /* Send ReadAddr high nibble address byte */
    FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
    /* Send ReadAddr medium nibble address byte */
    FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
    /* Send ReadAddr low nibble address byte */
    FLASH_SendByte(ReadAddr & 0xFF);
}

/*******************************************************************************
 * @brief  : Reads a byte from the SPI Flash.
 * @note   : This function must be used only if the Start_Read_Sequence function
 *           has been previously called.
 * @param  : None.
 * @retval : Byte read from the SPI Flash.
 ******************************************************************************/
uint8_t FLASH_ReadByte(void)
{
    return (FLASH_SendByte(FLASH_DUMMY_BYTE));
}

/*******************************************************************************
 * @brief  : Sends a byte through the SPI interface and return the byte received
 *           from the SPI bus.
 * @param  : byte: byte to send.
 * @retval : The value of the received byte.
 ******************************************************************************/
uint8_t FLASH_SendByte(uint8_t byte)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(FLASH_SPI, byte);
    
    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(FLASH_SPI);
}

/*******************************************************************************
 * @brief  : Sends a Half Word through the SPI interface and return the Half 
 *           Word received from the SPI bus.
 * @param  : HalfWord: Half Word to send.
 * @retval : The value of the received Half Word.
 ******************************************************************************/
uint16_t FLASH_SendHalfWord(uint16_t HalfWord)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send Half Word through the FLASH peripheral */
    SPI_I2S_SendData(FLASH_SPI, HalfWord);
    
    /* Wait to receive a Half Word */
    while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* Return the Half Word read from the SPI bus */
    return SPI_I2S_ReceiveData(FLASH_SPI);
}

/*******************************************************************************
 * @brief  : Enables the write access to the Flash.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_WriteEnable(void)
{
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send "Write Enable" instruction */
    FLASH_SendByte(FLASH_CMD_WREN);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Polls the status of the Write In Progress (WIP) flag in the Flash's
 *           status register and loop until write operation has completed.
 * @param  : None.
 * @retval : None.
 ******************************************************************************/
void FLASH_WaitForWriteEnd(void)
{
    uint8_t flashstatus = 0;
    
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    
    /* Send "Read Status Register" instruction */
    FLASH_SendByte(FLASH_CMD_RDSR);
    
    /* Loop as long as the memory is busy with a write cycle */
    do
    {
        /* Send a dummy byte to generate the clock needed by the FLASH
        and put the value of the status register in FLASH_Status variable */
        flashstatus = FLASH_SendByte(FLASH_DUMMY_BYTE);
    }
    while ((flashstatus & FLASH_WIP_FLAG) == SET); /* Write in progress */
    
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Write new value in Flash status register.
 * @param  : regval : new value of register.
 * @retval : None.
 ******************************************************************************/
void FLASH_WriteStatusRegister (uint8_t regval)
{
    FLASH_WriteEnable();
    
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send "WRSR " instruction */
    FLASH_SendByte(FLASH_CMD_WRSR);
    /* Read a byte from the FLASH */
    FLASH_SendByte(regval);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Read Flash status register.
 * @param  : None.
 * @retval : The value of the status register.
 ******************************************************************************/
uint8_t FLASH_ReadStatusRegister (void)
{
    uint8_t data;
    
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send "RDSR " instruction */
    FLASH_SendByte(FLASH_CMD_RDSR);
    /* Read a byte from the FLASH */
    data = FLASH_SendByte(FLASH_DUMMY_BYTE);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    return data;
}

/***************************************************************END OF FILE****/
