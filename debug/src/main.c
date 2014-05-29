/*******************************************************************************
 * main.c
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
#include "hw_config.h"


/* Private function prototypes -----------------------------------------------*/

static void PWR_Send(uint8_t reg, uint8_t val);
static void PWR_Receive(uint8_t reg, uint8_t *val);


/* Private define ------------------------------------------------------------*/
static const char HelpMsg[] =
    "[Flash]\n"
    "  f - Read the identifier of the Flash\n"
    "[Power Management]\n"
    "  pv - Show the battery voltage\n"
    "  pc - Show the battery current\n"
    "  pb - Show the Buck2 voltage\n"
    "  pr <reg> - Read the register value\n"
    "  pw <reg> <val> - Write the value into the register\n"
    "[Clock]\n"
    "  t - Show the actual time\n"
    "\n";


/*******************************************************************************
* Description    : Main routine.
* Input          : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
    char *ptr;
    char line[256];
    uint32_t id, voltage, current;
    uint8_t temp=0, hour, minute, second;
    long p1, p2;
    RTC_TimeTypeDef time;
    
    Set_System();
    
    // Configure the Power Management
    PWR_Init();
    PWR_EXTI_Init();
    
    // Configure LEDs and Slide
    Led_Config();
    Slide_Config();
    Led_Red_ON();
    
    // Configure the Flash memory
    FLASH_Init();
    
    // Enable the OLED High voltage
    //OLED_VDDH_Init();
    //OLED_VDDH_ON();
    
    // Configure the RTC Clock
    CLOCK_Init();
    CLOCK_EXTI_Init();
    CLOCK_Config(1200);
    
    // Configure the UART
    UART_Init(115200);
    xdev_out(UART_SendData);
    xdev_in(UART_ReceiveData);
    xputs("\nInfraModule UART debugger\nVersion 1.5\n\n");
    
    Led_Red_OFF();
    
    // Infinite loop
    while(1)
    {
        xputc('>');
        xgets(line, sizeof(line));
        
        ptr = line;
        switch (*ptr++)
        {
            case '?' :
                xputs(HelpMsg);
                break;
            
            case 'f' :
                id = FLASH_ReadID();
                xprintf("ID = 0x%02x\n", id);
                break;
            
            case 'p':
                switch (*ptr++)
                {
                    case 'v':
                        PWR_Send(0x0A, 0x8C);
                        delay_ms(10);
                        PWR_Receive(0x0B, &temp);
                        voltage = 2600 + (temp*1800/256);
                        xprintf("Voltage = %d mV\n", voltage);
                        PWR_Send(0x0A, 0x00);
                        break;
                    case 'c':
                        PWR_Send(0x0A, 0x0D);
                        delay_ms(10);
                        PWR_Receive(0x0B, &temp);
                        current = temp * 605/256;
                        xprintf("Current = %d mA\n", current);
                        PWR_Send(0x0A, 0x00);
                        break;
                    case 'b':
                        PWR_Receive(0x06, &temp);
                        voltage = 1700 + (temp&0x1F)*100;
                        if(voltage > 3300) voltage = 3300;
                        xprintf("Voltage = %d mV\n", voltage);
                        break;
                    case 'r':
                        if (!xatoi(&ptr, &p1)) break;
                        PWR_Receive((uint8_t)p1, &temp);
                        break;
                    case 'w':
                        if (!xatoi(&ptr, &p1) || !xatoi(&ptr, &p2)) break;
                        PWR_Send((uint8_t)p1, (uint8_t)p2);
                        PWR_Receive((uint8_t)p1, &temp);
                        break;
                }
                break;
            
            case 't':
                RTC_GetTime(RTC_Format_BIN, &time);
                hour   = (uint8_t)(time.RTC_Hours);
                minute = (uint8_t)(time.RTC_Minutes);
                second = (uint8_t)(time.RTC_Seconds);
                xprintf("Time : %dh%dm%ds\n", hour, minute, second); 
                break;
        }
    }
    
    return 0;
}


/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Description    : Change the value of the register.
* Input          : reg: register to change.
*                  val: value to implement.
* Return         : None.
*******************************************************************************/
void PWR_Send(uint8_t reg, uint8_t val)
{
    uint8_t error;
    error = PWR_WriteReg(reg, val);
    if(error)
        xprintf("[-] ERROR %d: Writing 0x%02x in reg 0x%02x.\n", error, val, reg);
    else
        xprintf("[+] Sending 0x%02x > (0x%02x).\n", val, reg);
}

/*******************************************************************************
* Description    : Read the value of the register.
* Input          : reg: register to read.
*                  val: value to return.
* Return         : None.
*******************************************************************************/
void PWR_Receive(uint8_t reg, uint8_t *val)
{
    uint8_t error;
    error = PWR_ReadReg(reg, val);
    if(error)
        xprintf("[-] error %d: Reading register 0x%02x.\n", error, reg);
    else
        xprintf("[+] Reading (0x%02x): 0x%02x.\n", reg, *val);
}

/***************************************************************END OF FILE****/
