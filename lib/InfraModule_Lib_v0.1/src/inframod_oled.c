/*******************************************************************************
 * inframod_oled.c
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
#include "inframod_oled.h"


/* Functions -----------------------------------------------------------------*/

/*******************************************************************************
 * @brief  : Configure the OLED.
 * @note   : Setup procedure according to the datasheet DD-160128FC-1A.
 * @param  : Configuration table.
 * @return : None.
 ******************************************************************************/
void OLED_Config(uint16_t *fconf)
{
    OLED_RESET_LOW();
    delay_ms(10);
    OLED_RESET_HIGH();
    delay_ms(10);
    
    /* display off, analog reset */
    OLED_WriteReg(0x04, 0x01);
    delay_ms(10);
    /* Power save mode : normal */
    OLED_WriteReg(0x04, 0x00);
    delay_ms(10);
    
    /* Display off */
    OLED_WriteReg(0x06, 0x00);
    
    /* Export1 internal clock and OSC operates with external resistor */
    OLED_WriteReg(0x02, 0x01);
    
    /* Iref controlled by external resistor */
    OLED_WriteReg(0x80, 0x00);
    
    if(fconf[0] == 0xFFFF)
    {
        /* DEFAULTS ----------------------------------------------------------*/
        
        /* Clock div ratio 1: freq setting 90Hz */
        OLED_WriteReg(0x03, 0x30);
        
        /* Driving current R G B - default R = 82uA / G = 56uA / B = 58uA */
        OLED_WriteReg(0x10, 0x52);     // 0x52
        OLED_WriteReg(0x11, 0x38);     // 0x38
        OLED_WriteReg(0x12, 0x3A);     // 0x3A
        
        /* Precharge time R G B */
        OLED_WriteReg(0x08, 0x01);     // 0x04 / 0x01
        OLED_WriteReg(0x09, 0x01);     // 0x05 / 0x01
        OLED_WriteReg(0x0A, 0x01);     // 0x05 / 0x01
        
        /* Precharge current R G B */
        OLED_WriteReg(0x0B, 0x0A);     // 0x9D / 0x0A
        OLED_WriteReg(0x0C, 0x0A);     // 0x8C / 0x0A
        OLED_WriteReg(0x0D, 0x0A);     // 0x57 / 0x0A
    }
    else
    {
        uint8_t freq;
        freq = (uint8_t)(fconf[1]/5) - 15;
        
        /* Frame Frequency */
        OLED_WriteReg(0x03, (freq << 8));
        
        /* Driving current R G B */
        OLED_WriteReg(0x10, (uint8_t)fconf[2]);
        OLED_WriteReg(0x11, (uint8_t)fconf[3]);
        OLED_WriteReg(0x12, (uint8_t)fconf[4]);
        
        /* Precharge time R G B */
        OLED_WriteReg(0x08, (uint8_t)fconf[5]);
        OLED_WriteReg(0x09, (uint8_t)fconf[6]);
        OLED_WriteReg(0x0A, (uint8_t)fconf[7]);
        
        /* Precharge current R G B */
        OLED_WriteReg(0x0B, (uint8_t)fconf[8]);
        OLED_WriteReg(0x0C, (uint8_t)fconf[9]);
        OLED_WriteReg(0x0D, (uint8_t)fconf[10]);
    }
    
    /* Display mode set :
     *  - RGB
     *  - Column = 0->159
     *  - Column data display = Normal display
     */
    OLED_WriteReg(0x13, 0x00);
    
    /* External interface mode=MPU */
    OLED_WriteReg(0x14, 0x31);     // 0x01 ?
    
    /* Memory write mode :
     *  - 8 bits dual transfer
     *  - 65K support
     *  - Horizontal address counter is increased
     *  - Vertical address counter is increased
     *  - The data is continuously written horizontally
     */
    OLED_WriteReg(0x16, 0x66);
    
    /* Duty = 128 */
    OLED_WriteReg(0x28, 0x7F);
    
    /* Display start on line 0 */
    OLED_WriteReg(0x29, 0x00);
    
    /* DDRAM read address start point 0x2E~0x2F */
    OLED_WriteReg(0x2E, 0x00);     // X
    OLED_WriteReg(0x2F, 0x00);     // Y
    
    /* Display screen saver size 0x33~0x36 */
    OLED_WriteReg(0x33, 0x00);     // Screen saver columns start
    OLED_WriteReg(0x34, 0x9F);     // Screen saver columns end
    OLED_WriteReg(0x35, 0x00);     // Screen saver row start
    OLED_WriteReg(0x36, 0x7F);     // Screen saver row end
    
    /* CLEAR SCREEN */
    OLED_Clear();
    
    OLED_VDDH_ON();
    delay_ms(100);
    
    /* Display ON */
    OLED_WriteReg(0x06, 0x01);
}

/*******************************************************************************
 * @brief  : Send a 8bits command to SEPS525.
 * @param  : cmd: command value.
 * @return : None.
 ******************************************************************************/
void OLED_SendCmd(uint8_t cmd)
{
    OLED_RS_LOW();
    OLED_CS_LOW();
    OLED_SPI_SendByte(cmd);
    OLED_CS_HIGH();
    OLED_RS_HIGH();
}

/*******************************************************************************
 * @brief  : Send a 16 bits data or value to SEPS525.
 * @param  : data: data value.
 * @return : None.
 ******************************************************************************/
void OLED_SendData(uint16_t data)
{
    OLED_SPI_SendByte((data>>8) & 0xFF);
    OLED_SPI_SendByte(data & 0xFF);
}

/*******************************************************************************
 * @brief  : Write a value in a register of SEPS525.
 * @param  : reg: register's name.
 *           val: register's value.
 * @return : None.
 ******************************************************************************/
void OLED_WriteReg(uint8_t reg, uint8_t val)
{
    OLED_CS_LOW();
    OLED_RS_LOW();
    OLED_SPI_SendByte(reg);
    OLED_RS_HIGH();
    OLED_CS_HIGH();
    
    OLED_CS_LOW();
    OLED_SPI_SendByte(val);
    OLED_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Start a data transfer in the SEPS525's memory.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_DataStart(void)
{
    OLED_CS_LOW();
    OLED_RS_LOW();
    OLED_SPI_SendByte(0x22);
    OLED_RS_HIGH();
}

/*******************************************************************************
 * @brief  : Stop a data transfer.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_DataEnd(void)
{
    OLED_CS_HIGH();
}

/*******************************************************************************
 * @brief  : Define a region in SEPS525's memory, to write data.
 * @param  : x: top left corner.
 *           y: top left corner.
 *           width: region's width.
 *           height: region's height.
 * @return : None.
 ******************************************************************************/
void OLED_SetRegion(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
{
    uint8_t x1, x2, y1, y2;
    
    x1 = x;
    y1 = y;
    x2 = x + width - 1;
    y2 = y + height - 1;
    
    // Valeurs limites
    if (x1 < 0) x1=0;
    if (x1 >= 160) x1=159;
    if (x2 < 0) x2=0;
    if (x2 >= 160) x2=159;
    if (y1 < 0) y1=0;
    if (y1 >= 128) y1=127;
    if (y2 < 0) y2=0;
    if (y2 >= 128) y2=127;
    
    OLED_WriteReg(0x17, x1);   // X start
    OLED_WriteReg(0x18, x2);   // X end
    OLED_WriteReg(0x19, y1);   // Y start
    OLED_WriteReg(0x1A, y2);   // Y end
    
    OLED_WriteReg(0x20, x1);   // memory accesspointer x
    OLED_WriteReg(0x21, y1);   // memory accesspointer y
}

/*******************************************************************************
 * @brief  : Draw a color rectangle on the screen.
 * @param  : x: top left corner.
 *           y: top left corner.
 *           width: rectangle's width.
 *           height: rectangle's height.
 *           color: rectangle's color.
 * @return : None.
 ******************************************************************************/
void OLED_Fill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t color)
{
    OLED_SetRegion(x, y, width, height);
    
    OLED_DataStart();
    uint16_t i;
    for (i=0; i<width*height; i++)
    {
        OLED_SendData(color);
    }
    OLED_DataEnd();
}

/*******************************************************************************
 * @brief  : Print the buffer's values on the screen.
 * @param  : x: top left corner.
 *           y: top left corner.
 *           width: buffer's width.
 *           height: buffer's height.
 *           buffer: buffer's values.
 * @return : None.
 ******************************************************************************/
void OLED_PrintBuffer(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint16_t *buffer)
{
    OLED_SetRegion(x, y, width, height);
    
    OLED_DataStart();
    uint16_t i;
    for (i=0; i<width*height; i++)
    {
        OLED_SendData(buffer[i]);
    }
    OLED_DataEnd();
}

/*******************************************************************************
 * @brief  : Clear the OLED screen.
 * @param  : None.
 * @return : None.
 ******************************************************************************/
void OLED_Clear(void)
{
    OLED_Fill(0, 0, 160, 128, 0x0000);
}

/***************************************************************END OF FILE****/
