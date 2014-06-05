/*******************************************************************************
 * inframod_pwr.c
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
#include "inframod_pwr.h"


/* Private prototypes --------------------------------------------------------*/
static void PWR_I2C_Init (void);
static void PWR_I2C_LowLevel_Init (void);

/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
 * @brief  : Initialize the Power Management LP3913.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void PWR_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    RCC_AHBPeriphClockCmd(  PWR_ONSTAT_GPIO_CLK | 
                            PWR_NRST_GPIO_CLK | 
                            PWR_IRQB_GPIO_CLK |
                            PWR_POWERACK_GPIO_CLK, 
                            ENABLE);
    
    /* ONSTAT pin (IN) */
    GPIO_InitStruct.GPIO_Pin   = PWR_ONSTAT_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(PWR_ONSTAT_GPIO_PORT, &GPIO_InitStruct);
    
    /* NRST pin (IN) */
    GPIO_InitStruct.GPIO_Pin   = PWR_NRST_PIN;
    GPIO_Init(PWR_NRST_GPIO_PORT, &GPIO_InitStruct);
    
    /* IRQB pin (IN) */
    GPIO_InitStruct.GPIO_Pin   = PWR_IRQB_PIN;
    GPIO_Init(PWR_IRQB_GPIO_PORT, &GPIO_InitStruct);
    
    /* POWERACK pin (OUT) */
    GPIO_InitStruct.GPIO_Pin   = PWR_POWERACK_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(PWR_POWERACK_GPIO_PORT, &GPIO_InitStruct);
    
    /* While NRST pin is in high state */
    while(!GPIO_ReadInputDataBit(PWR_NRST_GPIO_PORT, PWR_NRST_PIN))
    
    /* Startup Power Management */
    GPIO_HIGH(PWR_POWERACK_GPIO_PORT, PWR_POWERACK_PIN);
    
    /* Initialization of the I2C bus */
    PWR_I2C_Init();
    
    /* Voltage configuration of VCC_NUM */
    PWR_WriteReg(PWR_BUCK2_ADDR, PWR_BUCK2_VDD);
    
    /* Limit current configuration (100mA) */
    PWR_WriteReg(0x20, 0x01);
}

/*******************************************************************************
 * @brief  : Initialize the I2C peripheral.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void PWR_I2C_Init (void)
{
    I2C_InitTypeDef I2C_InitStruct;
    
    /* Initialize I2C port */
    PWR_I2C_LowLevel_Init();
    
    /* Deinitilize I2C */
    I2C_DeInit(PWR_I2C);
    
    /* Configure the I2C */
    I2C_InitStruct.I2C_Mode                = I2C_Mode_I2C;
    I2C_InitStruct.I2C_DutyCycle           = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1         = 0x00;
    I2C_InitStruct.I2C_Ack                 = I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_ClockSpeed          = PWR_I2C_SPEED;
    
    I2C_Init(PWR_I2C, &I2C_InitStruct);
    
    /* Enable the I2C */
    I2C_Cmd(PWR_I2C, ENABLE);
}

/*******************************************************************************
 * @brief  : Low level Initialization I2C peripheral.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void PWR_I2C_LowLevel_Init (void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    // Active l'horloge des I/O
    RCC_AHBPeriphClockCmd(PWR_I2C_SCL_GPIO_CLK | PWR_I2C_SDA_GPIO_CLK, ENABLE);
    
    // Active l'horloge de l'I2C
    RCC_APB1PeriphClockCmd(PWR_I2C_CLK, ENABLE);
    
    // Configure le peripherique I2C : SCL & SDA
    GPIO_PinAFConfig(PWR_I2C_SCL_GPIO_PORT, PWR_I2C_SCL_SOURCE, PWR_I2C_SCL_AF);
    GPIO_PinAFConfig(PWR_I2C_SDA_GPIO_PORT, PWR_I2C_SDA_SOURCE, PWR_I2C_SDA_AF);
    
    // Pin I2C_SCL :
    GPIO_InitStruct.GPIO_Pin   = PWR_I2C_SCL_PIN;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
    GPIO_Init(PWR_I2C_SCL_GPIO_PORT, &GPIO_InitStruct);
    
    // Pin I2C_SDA :
    GPIO_InitStruct.GPIO_Pin   = PWR_I2C_SDA_PIN;
    GPIO_Init(PWR_I2C_SDA_GPIO_PORT, &GPIO_InitStruct);
}

/*******************************************************************************
 * @brief  : Active l'interruption sur ONSTAT.
 * @param  : Aucun.
 * @return : Rien.
 ******************************************************************************/
void PWR_EXTI_Init (void)
{
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    // Demarrage de l'horloge SYSCFG :
    RCC_APB2PeriphClockCmd(RCC_APB2ENR_SYSCFGEN, ENABLE);
    
    // Cablage de EXTI_Line0 sur la pin ONSTAT:
    SYSCFG_EXTILineConfig(PWR_ONSTAT_EXTI_PORT, PWR_ONSTAT_SOURCE);
    
    // Configuration de EXTI
    EXTI_InitStruct.EXTI_Line    = PWR_ONSTAT_EXTI_LINE;
    EXTI_InitStruct.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);
    
    // Configuration de NVIC
    NVIC_InitStruct.NVIC_IRQChannel                   = PWR_ONSTAT_NVIC_IRQ;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

/*******************************************************************************
 * @brief  : Ecrit une valeur dans un registre avec le protocole I2C.
 * @param  : NomReg: numero du registre.
 *           ValeurReg: valeur a donner.
 * @return : 0 en cas de reussite, different en cas d'echec.
 ******************************************************************************/
uint8_t PWR_WriteReg (uint8_t Reg, uint8_t Value)
{
    uint32_t timeout = 0;
    uint8_t error = 0;
    
    // Test sur BUSY Flag
    Timed(I2C_GetFlagStatus(PWR_I2C, I2C_FLAG_BUSY));
    
    // Envoi du START
    I2C_GenerateSTART(PWR_I2C, ENABLE);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Transmet l'adresse de l'esclave et active le mode ECRITURE
    I2C_Send7bitAddress(PWR_I2C, (PWR_ADDR_ID<<1), I2C_Direction_Transmitter);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // Envoi de l'adresse du registre
    I2C_SendData(PWR_I2C, Reg);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    // Envoi de la donnee
    I2C_SendData(PWR_I2C, Value);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    // Envoi du STOP
    I2C_GenerateSTOP(PWR_I2C, ENABLE);
    Timed(I2C_GetFlagStatus(PWR_I2C, I2C_FLAG_STOPF));
    
    return 0;
}

/*******************************************************************************
 * @brief  : Lit une valeur dans un registre avec le protocole I2C.
 * @param  : NomReg: numero du registre.
 *           ValeurReg: valeur retournee.
 * @return : 0 en cas de reussite, different en cas d'echec.
 ******************************************************************************/
uint8_t PWR_ReadReg (uint8_t Reg, uint8_t *Value)
{
    uint32_t timeout = 0;
    uint8_t error = 0;
    *Value = 0;
    
    // Test sur BUSY Flag
    Timed(I2C_GetFlagStatus(PWR_I2C, I2C_FLAG_BUSY));
    
    // Envoi du START
    I2C_GenerateSTART(PWR_I2C, ENABLE);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Transmet l'adresse de l'esclave et active le mode ECRITURE
    I2C_Send7bitAddress(PWR_I2C, (PWR_ADDR_ID<<1), I2C_Direction_Transmitter);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    // Transmet le registre ou ecrire
    I2C_SendData(PWR_I2C, Reg);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    // Envoi d'un deuxieme START
    I2C_GenerateSTART(PWR_I2C, ENABLE);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_MODE_SELECT));
    
    // Transmet l'adresse de l'esclave et active le mode LECTURE
    I2C_Send7bitAddress(PWR_I2C, (PWR_ADDR_ID<<1), I2C_Direction_Receiver);
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
    
    // Desactive l'attente du ACK-Esclave
    I2C_AcknowledgeConfig(PWR_I2C, DISABLE);
    
    // Reception de la donnee
    Timed(!I2C_CheckEvent(PWR_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED));
    *Value = I2C_ReceiveData(PWR_I2C);
    
    // Envoi du STOP
    I2C_GenerateSTOP(PWR_I2C, ENABLE);
    Timed(I2C_GetFlagStatus(PWR_I2C, I2C_FLAG_STOPF));
    
    // Reactive l'attente du ACK-Esclave
    I2C_AcknowledgeConfig(PWR_I2C, ENABLE);
    
    return 0;
}

/***************************************************************END OF FILE****/
