/*******************************************************************************
 * inframod_oled.h
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __INFRAMOD_OLED_H
#define __INFRAMOD_OLED_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

#include "inframod_spi_oled.h"
#include "inframod_delay.h"


/* Exported functions --------------------------------------------------------*/

void OLED_Config (uint16_t *fconf);

/* Fonctions de Bas niveau */
void OLED_EnvoyerCmd (uint8_t Valeur);
void OLED_EnvoyerData (uint16_t Valeur);
void OLED_EcrireReg (uint8_t NomReg, uint8_t ValeurReg);
void OLED_DataStart (void);
void OLED_DataEnd (void);

/* Fonctions de Haut niveau */
void OLED_SetRegion (uint8_t x, uint8_t y, uint8_t largeur, uint8_t hauteur);
void OLED_Fill (uint8_t x, uint8_t y, uint8_t largeur, uint8_t hauteur, uint16_t couleur);
void OLED_AfficherBuffer (uint8_t x, uint8_t y, uint8_t largeur, uint8_t hauteur, uint16_t *buffer);
void OLED_Clear (void);


#endif  /*__INFRAMOD_OLED_H*/

/***************************************************************END OF FILE****/