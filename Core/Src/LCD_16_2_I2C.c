#include "LCD_16_2_I2C.h"
#include "string.h"
#include "stdio.h"

void LCD_16_2_I2C_Send_Cmd(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, uint8_t cmd)
{
	while (HAL_I2C_IsDeviceReady(lcd->hi2c, lcd->address, 1, HAL_MAX_DELAY) != HAL_OK)
	{

	}

	uint8_t upper_nibble, lower_nibble;
	uint8_t res[4];

	/* Extract upper nibble */
	upper_nibble = (cmd & 0xF0);

	/* Extract lower nibble */
	lower_nibble = ((cmd << 4) & 0xF0);

	res[0] = upper_nibble | LCD_16_2_I2C_BACK_LIGHT | LCD_16_2_I2C_EN_PIN;	/* EN = 1, RS = 0 */
	res[1] = upper_nibble | LCD_16_2_I2C_BACK_LIGHT; 						/* EN = 0, RS = 0 */
	res[2] = lower_nibble | LCD_16_2_I2C_BACK_LIGHT | LCD_16_2_I2C_EN_PIN; 	/* EN = 1, RS = 0 */
	res[3] = lower_nibble | LCD_16_2_I2C_BACK_LIGHT; 						/* EN = 0, RS = 0 */

	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, res, 4, 100);
}

void LCD_16_2_I2C_Send_Data(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, uint8_t data)
{
	while (HAL_I2C_IsDeviceReady(lcd->hi2c, lcd->address, 1, HAL_MAX_DELAY) != HAL_OK)
	{

	}

	uint8_t upper_nibble, lower_nibble;
	uint8_t res[4];

	/* Extract upper nibble */
	upper_nibble = (data & 0xF0);

	/* Extract lower nibble */
	lower_nibble = ((data << 4) & 0xF0);

	res[0] = upper_nibble | LCD_16_2_I2C_BACK_LIGHT | LCD_16_2_I2C_EN_PIN  | LCD_16_2_I2C_RS_PIN; 	/* EN = 1, RS = 1 */
	res[1] = upper_nibble | LCD_16_2_I2C_BACK_LIGHT; 												/* EN = 0, RS = 0 */
	res[2] = lower_nibble | LCD_16_2_I2C_BACK_LIGHT | LCD_16_2_I2C_EN_PIN  | LCD_16_2_I2C_RS_PIN; 	/* EN = 1, RS = 1 */
	res[3] = lower_nibble | LCD_16_2_I2C_BACK_LIGHT; 												/* EN = 0, RS = 0 */

	HAL_I2C_Master_Transmit(lcd->hi2c, lcd->address, res, 4, 100);
}

void LCD_16_2_I2C_Clear(st_I2C_LCD_16_2_HandleTypeDef_t *lcd)
{
	/* Move cursor to the home position */
	LCD_16_2_I2C_Send_Cmd(lcd, 0x80);

	for (int i = 0; i < 70; i++)
	{
		/* Write a space on each position */
		LCD_16_2_I2C_Send_Data(lcd, ' ');
	}
}

void LCD_16_2_I2C_GotoXY(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, int col, int row)
{
	if (row == 0)
	{
		col |= (0x00 + 0x80);	/* 1st row offset */
	}
	else
	{
		col |= (0x40 + 0x80);	/* 2nd row offset */
	}

	LCD_16_2_I2C_Send_Cmd(lcd, col);
}

void LCD_16_2_I2C_Init(st_I2C_LCD_16_2_HandleTypeDef_t *lcd)
{
	/* LCD configuration commands */
    LCD_16_2_I2C_Send_Cmd(lcd, 0x30);	/* 4 bits mode, 2 lines, 5x8 format */
    HAL_Delay(1);

    LCD_16_2_I2C_Send_Cmd(lcd, 0x02);  	/* Display and cursor home */
    HAL_Delay(1);

    LCD_16_2_I2C_Send_Cmd(lcd, 0x0C);	/* Display on, cursor off, blink off */
    HAL_Delay(2);

    LCD_16_2_I2C_Send_Cmd(lcd, 0x01);  	/* Clear display */
    HAL_Delay(1);
}

void LCD_16_2_I2C_WriteString(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, char *str)
{
	while (*str)
	{
		/* Send each character in the string */
		LCD_16_2_I2C_Send_Data(lcd, (uint8_t)(*str));
		str++;
	}
}

void LCD_16_2_I2C_WriteChar(st_I2C_LCD_16_2_HandleTypeDef_t *lcd, char c)
{
	/* Send the character to the display */
	LCD_16_2_I2C_Send_Data(lcd, (uint8_t)c);
}

void LCD_16_2_I2C_ScanAddress(st_I2C_LCD_16_2_HandleTypeDef_t *lcd)
{
	char info[] = "Scanning I2C address : ";

	HAL_UART_Transmit(lcd->huart, (uint8_t *)info, strlen(info), 1000);

	HAL_StatusTypeDef res;

	for (uint16_t i = 0; i < 128; i++)
	{
		res = HAL_I2C_IsDeviceReady(lcd->hi2c, i << 1, 1, 10);

		if (res == HAL_OK)
		{
			char msg[64];
			snprintf(msg, sizeof(msg), "0x%02X", i);
			HAL_UART_Transmit(lcd->huart, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);
		}
	}

	HAL_UART_Transmit(lcd->huart, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
}