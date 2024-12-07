#ifndef __LCD_16_2_I2C_H
#define __LCD_16_2_I2C_H

#include "stm32f4xx_hal.h"

#define LCD_16_2_I2C_RS_PIN		(1 << 0)
#define LCD_16_2_I2C_EN_PIN		(1 << 2)
#define LCD_16_2_I2C_BACK_LIGHT	(1 << 3)
#define LCD_16_2_I2C_ADDRESS	(0x3F << 1)


typedef struct
{
	I2C_HandleTypeDef 	*hi2c;
	UART_HandleTypeDef  *huart;
	uint8_t				address;
} st_I2C_LCD_16_2_HandleTypeDef_t;

void LCD_16_2_I2C_Init(st_I2C_LCD_16_2_HandleTypeDef_t *lcd);
void LCD_16_2_I2C_Send_Cmd(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, uint8_t cmd);
void LCD_16_2_I2C_Send_Data(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, uint8_t data);
void LCD_16_2_I2C_Clear(st_I2C_LCD_16_2_HandleTypeDef_t *lcd);
void LCD_16_2_I2C_GotoXY(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, int col, int row);
void LCD_16_2_I2C_WriteString(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, char *str);
void LCD_16_2_I2C_WriteChar(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, char c);
void LCD_16_2_I2C_ScanAddress(st_I2C_LCD_16_2_HandleTypeDef_t *lcd);

#endif	/* __LCD_16_2_I2C_H */
