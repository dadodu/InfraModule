/*******************************************************************************
 * inframod_spi_flash.c
 * 
 * Copyright 2013 Roman GAUCHI <roman.gauchi@gmail.com>
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
 *        +-----------------------------------------------------------+
 *        |                     Pin assignment                        |
 *        +-----------------------------+---------------+-------------+
 *        |  STM32 SPI Pins             |     FLASH     |    Pin      |
 *        +-----------------------------+---------------+-------------+
 *        | FLASH_CS_PIN                | ChipSelect(/S)|    1        |
 *        | FLASH_SPI_MISO_PIN / MISO   | DataOut(DQ1)  |    2        |
 *        |                             | Write(/W)     |    3        |
 *        |                             | GND           |    4 (0 V)  |
 *        | FLASH_SPI_MOSI_PIN / MOSI   | DataIn(DQ0)   |    5        |
 *        | FLASH_SPI_SCK_PIN / SCK     | Clock(C)      |    6        |
 *        |                             | /HOLD         |    7        |
 *        |                             | VCC           |    8 (3.3 V)|
 *        +-----------------------------+---------------+-------------+
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
 * @brief  : Desinitialise les peripheriques utilise par le SPI.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void FLASH_DeInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Desactive le FLASH_SPI  */
    SPI_Cmd(FLASH_SPI, DISABLE);
    
    /* Desinitialise le FLASH_SPI */
    SPI_I2S_DeInit(FLASH_SPI);
    
    /* Desactive l'horloge du peripherique FLASH_SPI */
    RCC_APB1PeriphClockCmd(FLASH_SPI_CLK, DISABLE);
    
    /* Configure la pin SCK */
    GPIO_InitStructure.GPIO_Pin  = FLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin SPI_MISO */
    GPIO_InitStructure.GPIO_Pin  = FLASH_SPI_MISO_PIN;
    GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin SPI_MOSI */
    GPIO_InitStructure.GPIO_Pin  = FLASH_SPI_MOSI_PIN;
    GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin CS */
    GPIO_InitStructure.GPIO_Pin  = FLASH_CS_PIN;
    GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 * @brief  : Initialise les peripheriques utilise par le SPI.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void FLASH_Init(void)
{
    SPI_InitTypeDef SPI_InitStructure;
    
    FLASH_HoldProtect_Init();
    FLASH_WriteProtect_Init();
    
    /* Initialisation des ports/pins */
    FLASH_LowLevel_Init();
    
    /* Selectionne la memoire FLASH */
    FLASH_CS_HIGH();
    
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
    SPI_Init(FLASH_SPI, &SPI_InitStructure);
    
    /* Active le FLASH_SPI */
    SPI_Cmd(FLASH_SPI, ENABLE);
}

/*******************************************************************************
 * @brief  : Initialise les pins utilisees par le SPI.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void FLASH_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Active les horloges des pins du SPI */
    RCC_AHBPeriphClockCmd(  FLASH_SPI_SCK_GPIO_CLK |
                            FLASH_SPI_MISO_GPIO_CLK | 
                            FLASH_SPI_MOSI_GPIO_CLK | 
                            FLASH_CS_GPIO_CLK, 
                            ENABLE);
    
    /* Active l'horloge du peripherique SPI */
    RCC_APB1PeriphClockCmd(FLASH_SPI_CLK, ENABLE);
    
    /* Configure la pin SCK */
    GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin SPI_MISO */
    GPIO_InitStructure.GPIO_Pin   = FLASH_SPI_MISO_PIN;
    GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
    
    /* Configure la pin SPI_MOSI */
    GPIO_InitStructure.GPIO_Pin = FLASH_SPI_MOSI_PIN;
    GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
    /* Connecte les pins SPI a leur fonctions alternatives */
    GPIO_PinAFConfig(FLASH_SPI_SCK_GPIO_PORT, FLASH_SPI_SCK_SOURCE, FLASH_SPI_SCK_AF);
    GPIO_PinAFConfig(FLASH_SPI_MISO_GPIO_PORT, FLASH_SPI_MISO_SOURCE, FLASH_SPI_MISO_AF);
    GPIO_PinAFConfig(FLASH_SPI_MOSI_GPIO_PORT, FLASH_SPI_MOSI_SOURCE, FLASH_SPI_MOSI_AF);
    
    /* Configure la pin CS */
    GPIO_InitStructure.GPIO_Pin   = FLASH_CS_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(FLASH_CS_GPIO_PORT, &GPIO_InitStructure);
}

/*******************************************************************************
 * @brief  : Initialise la pin HOLD.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void FLASH_HoldProtect_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Active l'horloge de la pin HOLD */
    RCC_AHBPeriphClockCmd(FLASH_HOLD_GPIO_CLK, ENABLE);
    
    /* Configure la pin HOLD */
    GPIO_InitStructure.GPIO_Pin   = FLASH_HOLD_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FLASH_HOLD_GPIO_PORT, &GPIO_InitStructure);
    
    /* Initialise la pin a l'etat haut */
    FLASH_HOLD_HIGH();
}

/*******************************************************************************
 * @brief  : Initialise la pin WR/Vpp.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void FLASH_WriteProtect_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* Active l'horloge de la pin WR */
    RCC_AHBPeriphClockCmd(FLASH_WR_GPIO_CLK, ENABLE);
    
    /* Configure la pin WR */
    GPIO_InitStructure.GPIO_Pin   = FLASH_WR_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(FLASH_HOLD_GPIO_PORT, &GPIO_InitStructure);
    
    /* Initialise la pin a l'etat haut */
    FLASH_WR_HIGH();
}

/*******************************************************************************
 * @brief  : Efface un sous-secteur (4096 octets = 0x1000) de la memoire FLASH.
 * @note   : quelquesoit l'adresse effacement par secteurs :
 *               [0x0000-0x0FFF], [0x1000-0x1FFF], [0x2000-0x2FFF] ...
 * @param  : Adresse du sous-secteur (inclue dans les intervalles).
 * @return : Rien.
 ******************************************************************************/
void FLASH_EraseSubsector(uint32_t SubsectorAddr)
{
    /* Send write enable instruction */
    FLASH_WriteEnable();
    
    /* Subsector Erase */
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send Subsector Erase instruction */
    FLASH_SendByte(FLASH_CMD_SSE);
    /* Send SubsectorAddr high nibble address byte */
    FLASH_SendByte((SubsectorAddr & 0xFF0000) >> 16);
    /* Send SubsectorAddr medium nibble address byte */
    FLASH_SendByte((SubsectorAddr & 0xFF00) >> 8);
    /* Send SubsectorAddr low nibble address byte */
    FLASH_SendByte(SubsectorAddr & 0xFF);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
    
    /* Wait the end of Flash writing */
    FLASH_WaitForWriteEnd();
}

/*******************************************************************************
 * @brief  : Efface un secteur (65536 octets = 0x10000) de la memoire FLASH.
 * @param  : Adresse du secteur.
 * @return : Rien.
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
 * @brief  : Efface completement la memoire FLASH.
 * @param  : Aucun.
 * @return : Rien.
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
 * @brief  : Ecrit une page en memoire (256 octets) en une seul fois.
 * @note   : le nombre d'octet ecrit ne peut pas depasser une page de FLASH.
 * @param  : pBuffer: pointeur sur le buffer contenant les donnees a ecrire en 
 *           memoire FLASH.
 * @param  : WriteAddr: addresse ou ecrire les donnees.
 * @param  : NumByteToWrite: nombre d'octet a ecrire en memoire FLASH, doit 
 *           etre egal ou inferieur a la taille d'une page de FLASH.
 * @return : Rien.
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
 * @brief  : Ecrit une page en memoire (4096 octets) en une seul fois.
 * @param  : pBuffer: pointeur sur le buffer contenant les donnees a ecrire en 
 *           memoire FLASH.
 * @param  : WriteAddr: addresse ou ecrire les donnees.
 * @param  : NumByteToWrite: nombre d'octet a ecrire en memoire FLASH.
 * @return : Rien.
 ******************************************************************************/
void FLASH_WriteSubsector(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t AddrSubsector;
    uint16_t i=0;
    uint8_t  Buffer[FLASH_SUBSECTOR_SIZE];
    
    AddrSubsector = (WriteAddr / FLASH_SUBSECTOR_SIZE) * FLASH_SUBSECTOR_SIZE;
    
    /* Lit toutes les donnees du sous-secteur */
    FLASH_ReadBuffer(Buffer, AddrSubsector, FLASH_SUBSECTOR_SIZE);
    
    /* Copie les donnees qui changent dans le nouveau buffer */
    while(NumByteToWrite--)
    {
        Buffer[i+(WriteAddr-AddrSubsector)] = *pBuffer;
        pBuffer++;
        i++;
    }
    
    /* Efface le sous-secteur */
    FLASH_EraseSubsector(AddrSubsector);
    
    /* Ecrit les nouvelles donnees dans le sous-secteur */
    for(i=0; i<0x10; i++)
    {
        FLASH_WritePage(Buffer+(i*0x100), AddrSubsector+(i*0x100), 0x100);
    }
}

/*******************************************************************************
 * @brief  : Ecrit un block de donnees en memoire. Dans cette fonction, le 
 *           nombre de cycle d'ecriture est reduit en utilisant FLASH_WritePage.
 * @param  : pBuffer: pointeur sur le buffer contenant les donnees a ecrire en 
 *           memoire FLASH.
 * @param  : WriteAddr: addresse ou ecrire les donnees.
 * @param  : NumByteToWrite: nombre d'octet a ecrire en memoire FLASH.
 * @return : Rien.
 ******************************************************************************/
void FLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfSubsector = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
    
    Addr = WriteAddr % FLASH_SUBSECTOR_SIZE;
    count = FLASH_SUBSECTOR_SIZE - Addr;
    NumOfSubsector =  NumByteToWrite / FLASH_SUBSECTOR_SIZE;
    NumOfSingle = NumByteToWrite % FLASH_SUBSECTOR_SIZE;
    
    if (Addr == 0) // WriteAddr is FLASH_SUBSECTOR_SIZE aligned
    {
        if (NumOfSubsector == 0) // NumByteToWrite < FLASH_SUBSECTOR_SIZE
        {
            FLASH_WriteSubsector(pBuffer, WriteAddr, NumByteToWrite);
        }
        else // NumByteToWrite > FLASH_SUBSECTOR_SIZE
        {
            while (NumOfSubsector--)
            {
                FLASH_WriteSubsector(pBuffer, WriteAddr, FLASH_SUBSECTOR_SIZE);
                WriteAddr +=  FLASH_SUBSECTOR_SIZE;
                pBuffer += FLASH_SUBSECTOR_SIZE;
            }
            
            FLASH_WriteSubsector(pBuffer, WriteAddr, NumOfSingle);
        }
    }
    else // WriteAddr is not FLASH_SUBSECTOR_SIZE aligned
    {
        if (NumOfSubsector == 0) // NumByteToWrite < FLASH_SUBSECTOR_SIZE
        {
            if (NumOfSingle > count) // (NumByteToWrite + WriteAddr) > FLASH_SUBSECTOR_SIZE
            {
                temp = NumOfSingle - count;
                
                FLASH_WriteSubsector(pBuffer, WriteAddr, count);
                WriteAddr += count;
                pBuffer += count;
                
                FLASH_WriteSubsector(pBuffer, WriteAddr, temp);
            }
            else
            {
                FLASH_WriteSubsector(pBuffer, WriteAddr, NumByteToWrite);
            }
        }
        else // NumByteToWrite > FLASH_SUBSECTOR_SIZE
        {
            NumByteToWrite -= count;
            NumOfSubsector =  NumByteToWrite / FLASH_SUBSECTOR_SIZE;
            NumOfSingle = NumByteToWrite % FLASH_SUBSECTOR_SIZE;
            
            FLASH_WriteSubsector(pBuffer, WriteAddr, count);
            WriteAddr +=  count;
            pBuffer += count;
            
            while (NumOfSubsector--)
            {
                FLASH_WriteSubsector(pBuffer, WriteAddr, FLASH_SUBSECTOR_SIZE);
                WriteAddr +=  FLASH_SUBSECTOR_SIZE;
                pBuffer += FLASH_SUBSECTOR_SIZE;
            }
            
            if (NumOfSingle != 0)
            {
                FLASH_WriteSubsector(pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/*******************************************************************************
 * @brief  : Lit un block de donnees en memoire FLASH.
 * @param  : pBuffer: pointeur sur le buffer qui recoit les donnees a lire en 
 *           memoire FLASH.
 * @param  : ReadAddr: addresse ou les donnees sont lues.
 * @param  : NumByteToRead: nombre d'octet a lire de la memoire FLASH.
 * @return : Rien.
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
 * @brief  : Lit l'identifiant de la memoire FLASH.
 * @param  : Aucun.
 * @return : Identification (ID) de la memoire FLASH.
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
 * @brief  : Initialise une sequence de lecture de donnees sur la Flash.
 * @param  : ReadAddr: adresse de lecture de depart dans la Flash.
 * @return : Rien.
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
 * @brief  : Lit un octet dans la Flash avec le SPI.
 * @note   : Cette fonction peut etre appelee seulement si la fonction 
 *           StartReadSequence a ete appelee.
 * @param  : Aucun.
 * @return : Octet lu par le SPI.
 ******************************************************************************/
uint8_t FLASH_ReadByte(void)
{
    return (FLASH_SendByte(FLASH_DUMMY_BYTE));
}

/*******************************************************************************
 * @brief  : Envoi un octet vers la Flash avec le SPI et retourne un octet lu.
 * @param  : byte: octet a envoyer.
 * @return : La valeur de l'octet recu.
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
 * @brief  : Envoi deux octets par le bus SPI et en retourne deux autres lus.
 * @param  : HalfWord: octets a envoyer.
 * @return : Valeur des octets recu.
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
 * @brief  : Active l'acces en lecture de la Flash.
 * @param  : Aucun.
 * @return : Rien.
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
 * @brief  : Attend que le processus d'ecriture dans la Flash se termine.
 * @param  : Aucun.
 * @return : Rien.
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
 * @brief  : Parametre le registre des status de la memoire FLASH.
 * @param  : Valeur a donner au registre des status.
 * @return : Rien.
 ******************************************************************************/
void FLASH_WriteStatusRegister (uint8_t valeur)
{
    FLASH_WriteEnable();
    
    /* Select the FLASH: Chip Select low */
    FLASH_CS_LOW();
    /* Send "WRSR " instruction */
    FLASH_SendByte(FLASH_CMD_WRSR);
    /* Read a byte from the FLASH */
    FLASH_SendByte(valeur);
    /* Deselect the FLASH: Chip Select high */
    FLASH_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Lit le registre des status de la memoire FLASH.
 * @param  : Aucun.
 * @return : Regiqtre des status.
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
