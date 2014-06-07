/*******************************************************************************
 * inframod_spi_oled.c
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
#include "inframod_spi_oled.h"


/* Privates prototypes -------------------------------------------------------*/
static void OLED_LowLevel_Init (void);
static void OLED_SPI_Init (void);
static void OLED_SPI_LowLevel_Init (void);

/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
 * @brief  : Initialize the OLED screen.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_Init (void)
{
    /* Low level Initialization of OLED ports */
    OLED_LowLevel_Init();
    
    /* Initialize the high voltage pin */
    OLED_VDDH_Init();
    
    /* Initialize the SPI peripheral */
    OLED_SPI_Init();
    
    /* 
     * OLED configuration :
     * RESET = 1    ('0' = reset SEPS525)
     * PS    = 0    ('0' = serial, '1' =  parallel bus)
     * CS    = 1    ('0' = SEPS525 accessible, 1 = SEPS525 lock)
     * RS    = 1    ('0' = commands, '1' = parameter/data)
     */
    OLED_RESET_LOW();
    
    GPIO_LOW(OLED_PS_PORT, OLED_PS_PIN);
    OLED_RS_HIGH();
    OLED_CS_HIGH();
    OLED_RESET_HIGH();
}

/*******************************************************************************
 * @brief  : Low level Initialization of the OLED ports.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* Enable I/O clocks */
    RCC_AHBPeriphClockCmd(  OLED_PS_CLK |
                            OLED_RS_CLK |
                            OLED_RESET_CLK,
                            ENABLE);
    
    /* Pins configuration */
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    
    /* Control pins */
    /* PS pin */
    GPIO_InitStruct.GPIO_Pin = OLED_PS_PIN;
    GPIO_Init(OLED_PS_PORT, &GPIO_InitStruct);
    
    /* RS pin */
    GPIO_InitStruct.GPIO_Pin = OLED_RS_PIN;
    GPIO_Init(OLED_RS_PORT, &GPIO_InitStruct);
    
    /* RESETB pin */
    GPIO_InitStruct.GPIO_Pin = OLED_RESET_PIN;
    GPIO_Init(OLED_RESET_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
 * @brief  : Initialize the OLED high voltage pin.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_VDDH_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* Enable clock */
    RCC_AHBPeriphClockCmd(OLED_VDDH_CLK, ENABLE);
    
    /* Configure the VDDH pin */
    GPIO_InitStruct.GPIO_Pin   = OLED_VDDH_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(OLED_VDDH_PORT, &GPIO_InitStruct);
    
    OLED_VDDH_OFF();
}

/*******************************************************************************
 * @brief  : Initialize the SPI peripheral.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_SPI_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
    /* Low level initialization */
    OLED_SPI_LowLevel_Init();
    
    /* SPI Configuration */
    SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL              = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA              = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial     = 7;
    SPI_Init(OLED_SPI, &SPI_InitStructure);
    
    /* Enable the SPI */
    SPI_Cmd(OLED_SPI, ENABLE);
}

/*******************************************************************************
 * @brief  : Low level initialization of SPI peripheral.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_SPI_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Enable clocks */
    RCC_AHBPeriphClockCmd(  OLED_CS_GPIO_CLK | 
                            OLED_SPI_MOSI_GPIO_CLK | 
                            OLED_SPI_MISO_GPIO_CLK | 
                            OLED_SPI_SCK_GPIO_CLK, 
                            ENABLE);
    
    /* Enable SPI clock */
    RCC_APB2PeriphClockCmd(OLED_SPI_CLK, ENABLE);
    
    /* SCK pin */
    GPIO_InitStructure.GPIO_Pin   = OLED_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(OLED_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI_MOSI pin */
    GPIO_InitStructure.GPIO_Pin   = OLED_SPI_MOSI_PIN;
    GPIO_Init(OLED_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* SPI_MISO pin */
    GPIO_InitStructure.GPIO_Pin   = OLED_SPI_MISO_PIN;
    GPIO_Init(OLED_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    /* Alternative functions */
    GPIO_PinAFConfig(OLED_SPI_SCK_GPIO_PORT, OLED_SPI_SCK_SOURCE, OLED_SPI_SCK_AF);
    GPIO_PinAFConfig(OLED_SPI_MOSI_GPIO_PORT, OLED_SPI_MOSI_SOURCE, OLED_SPI_MOSI_AF);
    GPIO_PinAFConfig(OLED_SPI_MISO_GPIO_PORT, OLED_SPI_MISO_SOURCE, OLED_SPI_MISO_AF);
    
    /* CS pin */
    GPIO_InitStructure.GPIO_Pin   = OLED_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(OLED_CS_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 * @brief  : Sends a byte through the SPI interface and return the byte received
 *           from the SPI bus.
 * @param  : byte: byte to send.
 * @return : The value of the received byte.
 ******************************************************************************/
uint8_t OLED_SPI_SendByte (uint8_t byte)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(OLED_SPI, byte);
    
    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(OLED_SPI);
}

/***************************************************************END OF FILE****/
