/*
 * ds3231.c
 *
 *  Created on: Apr 10, 2024
 *      Author: emanu
 */
#include "ds3231.h"
#include "main.h"
#include "string.h"
#include "stdio.h"

uint8_t data[BUFFER_SIZE];
char *Week[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

uint8_t BCD_Decimal(int value) {
	return (uint8_t) ((value / 16 * 10) + (value % 16));
}

uint8_t Decimal_BCD(int value) {
	return (uint8_t) ((value / 10 * 16) + (value % 10));
}

void SetUp(I2C_HandleTypeDef *hi2c) {
	data[0] = Decimal_BCD(0);
	data[1] = Decimal_BCD(39);
	data[2] = Decimal_BCD(9);
	data[3] = Decimal_BCD(4);
	data[4] = Decimal_BCD(23);
	data[5] = Decimal_BCD(5);
	data[6] = Decimal_BCD(24);

	HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c,
			(uint16_t) (ADDRESS_SLAVE), (uint16_t) (ADDRESS_SECONDS),
			ADDRESS_SIZE, data, BUFFER_SIZE, 100);
	if (status != HAL_OK) {
		uint32_t errorCode = HAL_I2C_GetError(hi2c);
	}
}

void Get_Date(Date_Time *dt, I2C_HandleTypeDef *hi2c) {

	HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c,
			(uint16_t) (ADDRESS_SLAVE), (uint16_t) (ADDRESS_SECONDS),
			ADDRESS_SIZE, (uint8_t*) data, BUFFER_SIZE, 1000);

	if (status == HAL_OK) {

		uint8_t values[BUFFER_SIZE];
		for (int i = 0; i < BUFFER_SIZE; i++) {
			values[i] = BCD_Decimal((int) data[i]);
		}

		dt->Seconds = values[0];
		dt->Minutes = values[1];
		dt->Hours = values[2];
		dt->Day = Week[values[3] - 1];
		dt->Date = values[4];
		dt->Month = values [5];
		dt->Year = (2000 + values[6]);
	}

}


