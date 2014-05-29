/*******************************************************************************
 * inframod_uart.c
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
#include "inframod_uart.h"


/*******************************************************************************
* Function Name  : UART_Init
* Description    : Initialize UART.
* Input          : baudRate: speed of the UART line.
* Return         : None.
*******************************************************************************/
void UART_Init(uint32_t baudRate)
{
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    
    /* Enable GPIO clock */
    RCC_AHBPeriphClockCmd(UART_RX_CLK | UART_TX_CLK, ENABLE);
    
    /* Enable USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    /* Configure USART Rx & Tx as alternate function */
    GPIO_InitStruct.GPIO_Pin    = UART_RX_PIN;
    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_40MHz;
    GPIO_InitStruct.GPIO_OType  = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd   = GPIO_PuPd_UP;
    GPIO_Init(UART_RX_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.GPIO_Pin    = UART_TX_PIN;
    GPIO_Init(UART_TX_PORT, &GPIO_InitStruct);
    
    /* Mux out USART1 Rx & Tx */
    GPIO_PinAFConfig(UART_TX_PORT, UART_TX_SOURCE, GPIO_AF_USART1);
    GPIO_PinAFConfig(UART_RX_PORT, UART_RX_SOURCE, GPIO_AF_USART1);
    
    /* USARTx configured as follow:
     * - Word Length = 8 Bits
     * - One Stop Bit
     * - No parity
     * - Hardware flow control not enabled (RTS and CTS signals)
     * - Receive and transmit enabled
    */
    USART_InitStruct.USART_BaudRate            = baudRate;
    USART_InitStruct.USART_WordLength          = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits            = USART_StopBits_1;
    USART_InitStruct.USART_Parity              = USART_Parity_No;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    
    /* USART configuration */
    USART_Init(UART, &USART_InitStruct);
    
    /* Enable UART */
    USART_Cmd(UART, ENABLE);
    
    /* Waitwhile TX full */
    while(USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
 * @brief  : Envoyer une donnees de 8bits en UART.
 * @param  : Aucun.
 * @retval : Rien.
 ******************************************************************************/
void UART_SendData(uint8_t data)
{
    // Envoi de la donnee
    USART_SendData(UART, data);
    
    // Attend que l'UART emette la donnee
    while(USART_GetFlagStatus(UART, USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
 * @brief  : Recevoir une donnees de 8bits en UART.
 * @param  : Aucun.
 * @retval : Rien.
 ******************************************************************************/
uint8_t UART_ReceiveData(void)
{
    // Attent que l'UART soit pret a recevoir
    while(USART_GetFlagStatus(UART, USART_FLAG_RXNE) == RESET);
    
    // lit la donnee sur 16bits
    return (uint8_t)(USART_ReceiveData(UART));
}


/***************************************************************END OF FILE****/
