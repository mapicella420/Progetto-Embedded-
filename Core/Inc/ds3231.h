/*
 * ds3231.h
 *
 *  Created on: Apr 10, 2024
 *      Author: emanu
 */

#ifndef INC_DS3231_H_
#define INC_DS3231_H_

#define ADDRESS_SLAVE 0xD0
#define ADDRESS_SECONDS 0x00
#define ADDRESS_MINUTES 0x01
#define ADDRESS_HOURS 0x02
#define ADDRESS_DAY 0x03
#define ADDRESS_DATE 0x04
#define ADDRESS_MONTH 0x05
#define ADDRESS_YEAR 0x06
#define ADDRESS_SIZE 1
#define BUFFER_SIZE 7

#include "main.h"


typedef struct{
	uint8_t Seconds;
	uint8_t Minutes;
	uint8_t Hours;
	char* Day;
	uint8_t Date;
	uint8_t Month;
	uint16_t Year;
}Date_Time;



uint8_t BCD_Decimal(int value);
uint8_t Decimal_BCD(int value);

void SetUp(I2C_HandleTypeDef* hi2c);
void Get_Date(Date_Time* dt, I2C_HandleTypeDef* hi2c);

#endif /* INC_DS3231_H_ */
