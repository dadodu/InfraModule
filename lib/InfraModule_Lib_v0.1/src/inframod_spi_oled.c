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
 * @brief  : Initialise et configure le peripherique I2C.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void OLED_Init (void)
{
    // Initialise les ports de l'OLED
    OLED_LowLevel_Init();
    
    // Initialise la pin d'alimentation
    OLED_VDDH_Init();
    
    // Initialise le peripherique SPI de l'OLED
    OLED_SPI_Init();
    
    /* 
     * Configuration de l'OLED :
     * RESET = 1    0:reset SEPS525
     * PS    = 0    0:bus serie 1:bus parallele
     * CS    = 1    0:SEPS525 accessible 1:SEPS525 verouille
     * RS    = 1    0:commande 1:parametre/data
     */
    OLED_RESET_LOW();
    
    GPIO_LOW(OLED_PS_PORT, OLED_PS_PIN);
    OLED_RS_HIGH();
    OLED_CS_HIGH();
    OLED_RESET_HIGH();
}

/*******************************************************************************
 * @brief  : Initialise et configure les ports materiel de l'OLED.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void OLED_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // Active les horloge des I/O :
    RCC_AHBPeriphClockCmd(  OLED_PS_CLK |
                            OLED_RS_CLK |
                            OLED_RESET_CLK,
                            ENABLE);
    
    // Configure les pins en Sortie, Push-Pull, sans Pull-Up a 40 MHz
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    
    /* Pins de controle */
    // Pin PS
    GPIO_InitStruct.GPIO_Pin = OLED_PS_PIN;
    GPIO_Init(OLED_PS_PORT, &GPIO_InitStruct);
    
    // Pin RS
    GPIO_InitStruct.GPIO_Pin = OLED_RS_PIN;
    GPIO_Init(OLED_RS_PORT, &GPIO_InitStruct);
    
    // Pin RESETB
    GPIO_InitStruct.GPIO_Pin = OLED_RESET_PIN;
    GPIO_Init(OLED_RESET_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
 * @brief  : Initialise et configure la broche d'alimentation haute de l'OLED.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void OLED_VDDH_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // Active les horloge des I/O :
    RCC_AHBPeriphClockCmd(OLED_VDDH_CLK, ENABLE);
    
    // Configure la pin en Sortie, Push-Pull, sans Pull-Up a 40 MHz
    GPIO_InitStruct.GPIO_Pin   = OLED_VDDH_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(OLED_VDDH_PORT, &GPIO_InitStruct);
    
    OLED_VDDH_OFF();
}

/*******************************************************************************
 * @brief  : Initialise les peripheriques utilise par le SPI.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void OLED_SPI_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
    /* Initialisation des ports/pins */
    OLED_SPI_LowLevel_Init();
    
    /* Configuration du SPI */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(OLED_SPI, &SPI_InitStructure);
    
    /* Active le OLED_SPI */
    SPI_Cmd(OLED_SPI, ENABLE);
}

/*******************************************************************************
 * @brief  : Initialise les pins utilisees par le SPI.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void OLED_SPI_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Active les horloges des pins du SPI */
    RCC_AHBPeriphClockCmd(  OLED_CS_GPIO_CLK | 
                            OLED_SPI_MOSI_GPIO_CLK | 
                            OLED_SPI_MISO_GPIO_CLK | 
                            OLED_SPI_SCK_GPIO_CLK, 
                            ENABLE);
    
    /* Active l'horloge du peripherique SPI */
    RCC_APB2PeriphClockCmd(OLED_SPI_CLK, ENABLE);
    
    /* Configure la pin SCK */
    GPIO_InitStructure.GPIO_Pin   = OLED_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(OLED_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin SPI_MOSI */
    GPIO_InitStructure.GPIO_Pin = OLED_SPI_MOSI_PIN;
    GPIO_Init(OLED_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* Connecte les pins SPI a leur fonctions alternatives */
    GPIO_PinAFConfig(OLED_SPI_SCK_GPIO_PORT, OLED_SPI_SCK_SOURCE, OLED_SPI_SCK_AF);
    GPIO_PinAFConfig(OLED_SPI_MISO_GPIO_PORT, OLED_SPI_MOSI_SOURCE, OLED_SPI_MOSI_AF);
    
    /* Configure la pin SPI_MISO */
    GPIO_InitStructure.GPIO_Pin = OLED_SPI_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_Init(OLED_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin CS */
    GPIO_InitStructure.GPIO_Pin   = OLED_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(OLED_CS_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 * @brief  : Envoi un octet pendant que l'interface SPI recoit un octet.
 * @param  : octet: octet a envoyer.
 * @return : la valeur de l'octet recue.
 ******************************************************************************/
uint8_t OLED_SPI_EnvoyerOctet (uint8_t octet)
{
    /* Loop while DR register in not emplty */
    while (SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(OLED_SPI, octet);
    
    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(OLED_SPI, SPI_I2S_FLAG_RXNE) == RESET);
    
    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(OLED_SPI);
}

/***************************************************************END OF FILE****/
