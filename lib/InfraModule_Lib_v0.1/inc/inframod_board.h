/*******************************************************************************
 * inframod_board.h
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
 ******************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __INFRAMOD_BOARD_H
#define __INFRAMOD_BOARD_H

/* Set, Reset and Toggle */
#define GPIO_HIGH(a,b)                  a->BSRRL = b
#define GPIO_LOW(a,b)                   a->BSRRH = b
#define GPIO_TOGGLE(a,b)                a->ODR ^= b

/******************************************************************************/
/*                  DEFINITIONS OF CONSTANTS FOR VERSION 1.5                  */
/******************************************************************************/

/* Inputs/Outputs */
#define SLIDE_1_PIN                     GPIO_Pin_12                 // PC12
#define SLIDE_1_GPIO_PORT               GPIOC
#define SLIDE_1_GPIO_CLK                RCC_AHBPeriph_GPIOC
#define SLIDE_2_PIN                     GPIO_Pin_15                 // PA15
#define SLIDE_2_GPIO_PORT               GPIOA
#define SLIDE_2_GPIO_CLK                RCC_AHBPeriph_GPIOA

#define LED_GREEN_PIN                   GPIO_Pin_2                  // PA2
#define LED_GREEN_PORT                  GPIOA
#define LED_GREEN_CLK                   RCC_AHBPeriph_GPIOA
#define LED_RED_PIN                     GPIO_Pin_3                  // PA3
#define LED_RED_PORT                    GPIOA
#define LED_RED_CLK                     RCC_AHBPeriph_GPIOA


/* Power Management LP3913 - I2C */
#define PWR_I2C                         I2C2
#define PWR_I2C_CLK                     RCC_APB1Periph_I2C2
#define PWR_I2C_SCL_PIN                 GPIO_Pin_10                 // PB10
#define PWR_I2C_SCL_GPIO_PORT           GPIOB
#define PWR_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define PWR_I2C_SCL_SOURCE              GPIO_PinSource10
#define PWR_I2C_SCL_AF                  GPIO_AF_I2C2
#define PWR_I2C_SDA_PIN                 GPIO_Pin_11                 // PB11
#define PWR_I2C_SDA_GPIO_PORT           GPIOB
#define PWR_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define PWR_I2C_SDA_SOURCE              GPIO_PinSource11
#define PWR_I2C_SDA_AF                  GPIO_AF_I2C2

#define PWR_ONSTAT_PIN                  GPIO_Pin_0                  // PB0
#define PWR_ONSTAT_GPIO_PORT            GPIOB
#define PWR_ONSTAT_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define PWR_ONSTAT_EXTI_PORT            EXTI_PortSourceGPIOB
#define PWR_ONSTAT_EXTI_LINE            EXTI_Line0
#define PWR_ONSTAT_NVIC_IRQ             EXTI0_IRQn
#define PWR_ONSTAT_SOURCE               EXTI_PinSource0
#define PWR_POWERACK_PIN                GPIO_Pin_1                  // PA1
#define PWR_POWERACK_GPIO_PORT          GPIOA
#define PWR_POWERACK_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define PWR_NRST_PIN                    GPIO_Pin_7                  // PA7
#define PWR_NRST_GPIO_PORT              GPIOA
#define PWR_NRST_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define PWR_IRQB_PIN                    GPIO_Pin_6                  // PA6
#define PWR_IRQB_GPIO_PORT              GPIOA
#define PWR_IRQB_GPIO_CLK               RCC_AHBPeriph_GPIOA
#define PWR_IRQB_EXTI_PORT              EXTI_PortSourceGPIOA
#define PWR_IRQB_SOURCE                 EXTI_PinSource6


/* Flash memory N25Q32 - SPI */
#define FLASH_SPI                       SPI2
#define FLASH_SPI_CLK                   RCC_APB1Periph_SPI2
#define FLASH_CS_PIN                    GPIO_Pin_12                 // PB12
#define FLASH_CS_GPIO_PORT              GPIOB
#define FLASH_CS_GPIO_CLK               RCC_AHBPeriph_GPIOB
#define FLASH_SPI_SCK_PIN               GPIO_Pin_13                 // PB13
#define FLASH_SPI_SCK_GPIO_PORT         GPIOB
#define FLASH_SPI_SCK_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define FLASH_SPI_SCK_SOURCE            GPIO_PinSource13
#define FLASH_SPI_SCK_AF                GPIO_AF_SPI2
#define FLASH_SPI_MISO_PIN              GPIO_Pin_14                 // PB14
#define FLASH_SPI_MISO_GPIO_PORT        GPIOB
#define FLASH_SPI_MISO_GPIO_CLK         RCC_AHBPeriph_GPIOB
#define FLASH_SPI_MISO_SOURCE           GPIO_PinSource14
#define FLASH_SPI_MISO_AF               GPIO_AF_SPI2
#define FLASH_SPI_MOSI_PIN              GPIO_Pin_15                 // PB15
#define FLASH_SPI_MOSI_GPIO_PORT        GPIOB
#define FLASH_SPI_MOSI_GPIO_CLK         RCC_AHBPeriph_GPIOB
#define FLASH_SPI_MOSI_SOURCE           GPIO_PinSource15
#define FLASH_SPI_MOSI_AF               GPIO_AF_SPI2

#define FLASH_HOLD_PIN                  GPIO_Pin_8                  // PA8
#define FLASH_HOLD_GPIO_PORT            GPIOA
#define FLASH_HOLD_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define FLASH_WR_PIN                    GPIO_Pin_9                  // PC9
#define FLASH_WR_GPIO_PORT              GPIOC
#define FLASH_WR_GPIO_CLK               RCC_AHBPeriph_GPIOC


/* OLED screen DD-160128FC-1A */

// Configuration pins
#define OLED_CPU_PIN                    GPIO_Pin_10                 // PC10
#define OLED_CPU_PORT                   GPIOC
#define OLED_CPU_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_PS_PIN                     GPIO_Pin_11                 // PC11
#define OLED_PS_PORT                    GPIOC
#define OLED_PS_CLK                     RCC_AHBPeriph_GPIOC

// Control pins
#define OLED_RS_PIN                     GPIO_Pin_9                  // PB9
#define OLED_RS_PORT                    GPIOB
#define OLED_RS_CLK                     RCC_AHBPeriph_GPIOB
#define OLED_CS_PIN                     GPIO_Pin_4                  // PA4
#define OLED_CS_PORT                    GPIOA
#define OLED_CS_CLK                     RCC_AHBPeriph_GPIOA
#define OLED_RD_PIN                     GPIO_Pin_2                  // PD2
#define OLED_RD_PORT                    GPIOD
#define OLED_RD_CLK                     RCC_AHBPeriph_GPIOD
#define OLED_WR_PIN                     GPIO_Pin_0                  // PA0
#define OLED_WR_PORT                    GPIOA
#define OLED_WR_CLK                     RCC_AHBPeriph_GPIOA
#define OLED_RESET_PIN                  GPIO_Pin_13                 // PC13
#define OLED_RESET_PORT                 GPIOC
#define OLED_RESET_CLK                  RCC_AHBPeriph_GPIOC

// Data pins - Parallel bus
#define OLED_D9_PIN                     GPIO_Pin_0                  // PC0
#define OLED_D9_PORT                    GPIOC
#define OLED_D9_CLK                     RCC_AHBPeriph_GPIOC
#define OLED_D10_PIN                    GPIO_Pin_1                  // PC1
#define OLED_D10_PORT                   GPIOC
#define OLED_D10_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D11_PIN                    GPIO_Pin_2                  // PC2
#define OLED_D11_PORT                   GPIOC
#define OLED_D11_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D12_PIN                    GPIO_Pin_3                  // PC3
#define OLED_D12_PORT                   GPIOC
#define OLED_D12_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D13_PIN                    GPIO_Pin_4                  // PC4
#define OLED_D13_PORT                   GPIOC
#define OLED_D13_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D14_PIN                    GPIO_Pin_5                  // PC5
#define OLED_D14_PORT                   GPIOC
#define OLED_D14_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D15_PIN                    GPIO_Pin_6                  // PC6
#define OLED_D15_PORT                   GPIOC
#define OLED_D15_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D16_PIN                    GPIO_Pin_7                  // PC7
#define OLED_D16_PORT                   GPIOC
#define OLED_D16_CLK                    RCC_AHBPeriph_GPIOC
#define OLED_D17_PIN                    GPIO_Pin_8                  // PC8
#define OLED_D17_PORT                   GPIOC
#define OLED_D17_CLK                    RCC_AHBPeriph_GPIOC

// Data pins - SPI bus
#define OLED_SPI                        SPI1
#define OLED_SPI_CLK                    RCC_APB2Periph_SPI1
#define OLED_CS_PIN                     GPIO_Pin_4                  // PA4
#define OLED_CS_GPIO_PORT               GPIOA
#define OLED_CS_GPIO_CLK                RCC_AHBPeriph_GPIOA
#define OLED_SPI_SCK_PIN                GPIO_Pin_3                  // PB3 (D17)
#define OLED_SPI_SCK_GPIO_PORT          GPIOB
#define OLED_SPI_SCK_GPIO_CLK           RCC_AHBPeriph_GPIOB
#define OLED_SPI_SCK_SOURCE             GPIO_PinSource3
#define OLED_SPI_SCK_AF                 GPIO_AF_SPI1
#define OLED_SPI_MISO_PIN               GPIO_Pin_4                  // PB4 (D15)
#define OLED_SPI_MISO_GPIO_PORT         GPIOB
#define OLED_SPI_MISO_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define OLED_SPI_MISO_SOURCE            GPIO_PinSource4
#define OLED_SPI_MISO_AF                GPIO_AF_SPI1
#define OLED_SPI_MOSI_PIN               GPIO_Pin_5                  // PB5 (D16)
#define OLED_SPI_MOSI_GPIO_PORT         GPIOB
#define OLED_SPI_MOSI_GPIO_CLK          RCC_AHBPeriph_GPIOB
#define OLED_SPI_MOSI_SOURCE            GPIO_PinSource5
#define OLED_SPI_MOSI_AF                GPIO_AF_SPI1

// Supply pins VCC_OLED
#define OLED_VDDH_PIN                   GPIO_Pin_5                  // PA5
#define OLED_VDDH_CLK                   RCC_AHBPeriph_GPIOA
#define OLED_VDDH_PORT                  GPIOA


/* UART debugger : TTL-232R-3V3 */
#define UART                            USART1
#define UART_TX_PIN                     GPIO_Pin_9                  // PA9 (TX)
#define UART_TX_PORT                    GPIOA
#define UART_TX_CLK                     RCC_AHBPeriph_GPIOA
#define UART_TX_SOURCE                  GPIO_PinSource9
#define UART_RX_PIN                     GPIO_Pin_10                 // PA10 (RX)
#define UART_RX_PORT                    GPIOA
#define UART_RX_CLK                     RCC_AHBPeriph_GPIOA
#define UART_RX_SOURCE                  GPIO_PinSource10



#endif /*__INFRAMOD_BOARD_H*/

/***************************************************************END OF FILE****/
