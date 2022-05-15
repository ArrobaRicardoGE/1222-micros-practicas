#include "I2C.h"
#include "lcd.h"

void I2C_Init()												/* I2C initialize function */
{
	TWBR = BITRATE(TWSR = 0x00);							/* Get bit rate register value by formula */
}


uint8_t I2C_Start(char write_address)						/* I2C start function */
{
	uint8_t status;											/* Declare variable */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Enable TWI, generate start condition and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (start condition) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status != 0x08)										/* Check weather start condition transmitted successfully or not? */
	return 0;												/* If not then return 0 to indicate start condition fail */
	TWDR = write_address;									/* If yes then write SLA+W in TWI data register */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status == 0x18)										/* Check weather SLA+W transmitted & ack received or not? */
	return 1;												/* If yes then return 1 to indicate ack received i.e. ready to accept data byte */
	if (status == 0x20)										/* Check weather SLA+W transmitted & nack received or not? */
	return 2;												/* If yes then return 2 to indicate nack received i.e. device is busy */
	else
	return status;												/* Else return 3 to indicate SLA+W failed */
}

uint8_t I2C_Repeated_Start(char read_address)				/* I2C repeated start function */
{
	uint8_t status;											/* Declare variable */
	TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);					/* Enable TWI, generate start condition and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (start condition) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status != 0x10)										/* Check weather repeated start condition transmitted successfully or not? */
	return 0;												/* If no then return 0 to indicate repeated start condition fail */
	TWDR = read_address;									/* If yes then write SLA+R in TWI data register */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status == 0x40)										/* Check weather SLA+R transmitted & ack received or not? */
	return 1;												/* If yes then return 1 to indicate ack received */
	if (status == 0x20)										/* Check weather SLA+R transmitted & nack received or not? */
	return 2;												/* If yes then return 2 to indicate nack received i.e. device is busy */
	else
	return 3;												/* Else return 3 to indicate SLA+W failed */
}

void I2C_Stop()												/* I2C stop function */
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					/* Enable TWI, generate stop condition and clear interrupt flag */
	while(TWCR & (1<<TWSTO));								/* Wait until stop condition execution */
}

void I2C_Start_Wait(char write_address)						/* I2C start wait function */
{
	uint8_t status;											/* Declare variable */
	while (1)
	{
		TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);				/* Enable TWI, generate start condition and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (start condition) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x08)									/* Check weather start condition transmitted successfully or not? */
		continue;											/* If no then continue with start loop again */
		TWDR = write_address;								/* If yes then write SLA+W in TWI data register */
		TWCR = (1<<TWEN)|(1<<TWINT);						/* Enable TWI and clear interrupt flag */
		while (!(TWCR & (1<<TWINT)));						/* Wait until TWI finish its current job (Write operation) */
		status = TWSR & 0xF8;								/* Read TWI status register with masking lower three bits */
		if (status != 0x18 )								/* Check weather SLA+W transmitted & ack received or not? */
		{
			I2C_Stop();										/* If not then generate stop condition */
			continue;										/* continue with start loop again */
		}
		break;												/* If yes then break loop */
	}
}

uint8_t I2C_Write(char data)								/* I2C write function */
{
	uint8_t status;											/* Declare variable */
	TWDR = data;											/* Copy data in TWI data register */
	TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	if (status == 0x28)										/* Check weather data transmitted & ack received or not? */
	return 0;												/* If yes then return 0 to indicate ack received */
	if (status == 0x30)										/* Check weather data transmitted & nack received or not? */
	return 1;												/* If yes then return 1 to indicate nack received */
	else
	return 2;												/* Else return 2 to indicate data transmission failed */
}

char I2C_Read_Ack()											/* I2C read ack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);					/* Enable TWI, generation of ack and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation) */
	return TWDR;											/* Return received data */
}

char I2C_Read_Nack()										/* I2C read nack function */
{
	TWCR=(1<<TWEN)|(1<<TWINT);								/* Enable TWI and clear interrupt flag */
	while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation) */
	return TWDR;											/* Return received data */
}

//void show_number2(uint8_t x) {
	////for(int i = 0; i < ret; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL);
	//int pad = 3;
	//char ans[pad + 1];
	//ans[pad--] = 0;
	//while(x > 0) {
		//ans[pad--] = (x % 10) + '0';
		//x /= 10;
	//}
	//while(pad >= 0) {
		//ans[pad--] = '0';
	//}
	//LCD_WR_string(ans);
	////for(int i = 0; i < ret - 3; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR);
//}
//
//void I2C_Init() {
	////TWCR = (1<<TWEN)|(1<<TWINT);
	//TWBR = BITRATE(TWSR = 0x00); // bit rate and status register
//}
//
//uint8_t I2C_Start(uint8_t write_address) {
	//uint8_t status;											/* Declare variable */
	//TWCR = (1<<TWSTA)|(1<<TWEN)|(1<<TWINT);			 		/* Enable TWI, generate start condition and clear interrupt flag */
	//while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (start condition) */
	//status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	//LCD_WR_string("Init status ");
	//show_number2(status); 
	//_delay_ms(1000);
	//LCD_CMD_8BIT(LCD_Cmd_Clear); 
	//if (status != 0x08)										/* Check weather start condition transmitted successfully or not? */
	//return 0;												/* If not then return 0 to indicate start condition fail */
	//
	//show_number2(write_address);
	//_delay_ms(1000);
	//LCD_CMD_8BIT(LCD_Cmd_Clear);
	//TWDR = 0x29;									/* If yes then write SLA+W in TWI data register */
	//TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	//while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	//status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	//LCD_WR_string("set addr ");
	//show_number2(status); 
	//_delay_ms(1000);
	//LCD_CMD_8BIT(LCD_Cmd_Clear);
	//if (status == 0x18)										/* Check weather SLA+W transmitted & ack received or not? */
	//return 1;												/* If yes then return 1 to indicate ack received i.e. ready to accept data byte */
	//if (status == 0x20)										/* Check weather SLA+W transmitted & nack received or not? */
	//return 2;												/* If yes then return 2 to indicate nack received i.e. device is busy */
	//else
	//return 3;
//}
//
//void I2C_Stop()												/* I2C stop function */
//{
	//TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);					/* Enable TWI, generate stop condition and clear interrupt flag */
	//while(TWCR & (1<<TWSTO));								/* Wait until stop condition execution */
//}
//
//uint8_t I2C_Write(uint8_t data)								/* I2C write function */
//{
	//uint8_t status;											/* Declare variable */
	//TWDR = data;											/* Copy data in TWI data register */
	//TWCR = (1<<TWEN)|(1<<TWINT);							/* Enable TWI and clear interrupt flag */
	//while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (Write operation) */
	//status = TWSR & 0xF8;									/* Read TWI status register with masking lower three bits */
	//if (status == 0x28)										/* Check weather data transmitted & ack received or not? */
	//return 0;												/* If yes then return 0 to indicate ack received */
	//if (status == 0x30)										/* Check weather data transmitted & nack received or not? */
	//return 1;												/* If yes then return 1 to indicate nack received */
	//else
	//return 2;												/* Else return 2 to indicate data transmission failed */
//}
//
//uint8_t I2C_Read_Ack()											/* I2C read ack function */
//{
	//TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);					/* Enable TWI, generation of ack and clear interrupt flag */
	//while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation) */
	//return TWDR;											/* Return received data */
//}
//
//uint8_t I2C_Read_Nack()										/* I2C read nack function */
//{
	//TWCR=(1<<TWEN)|(1<<TWINT);								/* Enable TWI and clear interrupt flag */
	//while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation) */
	//return TWDR;											/* Return received data */
//}
