/*
 * Final.c
 *
 * Created: 08/05/2022 04:25:50 p. m.
 * Author : Ricardo Gutierrez
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "util.h"
#include "lcd.h"
#include "I2C.h"

#define PCS_WRITE_ADDR (0x29 << 1)
#define PCS_READ_ADDR ((0x29 << 1) + 1)

void show_number(uint8_t x) {
	//for(int i = 0; i < ret; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL); 
	int pad = 3; 
	char ans[pad + 1]; 
	ans[pad--] = 0;
	while(x > 0) {
		ans[pad--] = (x % 10) + '0';
		x /= 10;
	}
	while(pad >= 0) {
		ans[pad--] = '0'; 
	}
	LCD_WR_string(ans); 
	//for(int i = 0; i < ret - 3; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
}

void show_rgb(uint8_t r, uint8_t g, uint8_t b) {
	for(int i = 0; i < 14; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL); 
	show_number(r);  
	for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
	show_number(g);
	for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
	show_number(b);
	for(int i = 0; i < 1; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
}

uint8_t read8(uint8_t reg) {
	//LCD_WR_CHAR('A'); 
	//uint8_t val = I2C_Start(0x29); // Specified in datasheet
	//show_number(val);
	//_delay_ms(1000); 
	//LCD_CMD_8BIT(LCD_Cmd_Clear);
	//I2C_Stop(); 
	uint8_t val = I2C_Start(0x29); 
	show_number(val);
	uint8_t req = (0x80 | reg); // command bit
	I2C_Write(req); 
	val = I2C_Read_Ack(); 
	I2C_Stop();
	return val;
}

void start_condition() {
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN); 
	while(!(TWCR & (1 << TWINT))); 
	uint8_t status = TWSR & 0xF8; 
	//show_number(status); 
}

void set_addr(uint8_t addr) {
	TWDR = addr; 
	TWCR = (1 << TWINT)|(1 << TWEN);
	while(!(TWCR & (1 << TWINT))) {
		//LCD_WR_CHAR('a'); 
	}
	//show_number(TWSR & 0xF8); 
}

void write_data(uint8_t data) {
	TWDR = data; 
	TWCR = (1 << TWINT)|(1 << TWEN); 
	while(!(TWCR & (1 << TWINT))); 
	//show_number(TWSR & 0xF8); 
}

void stop_condition() {
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO); 
}

uint8_t read_nack() {
	TWCR=(1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	return TWDR; 
}

uint8_t read_cmd_8(uint8_t cmd) {
	start_condition(); 
	set_addr(PCS_WRITE_ADDR); 
	write_data((0x80 | cmd));	// mask with command byte
	start_condition();			// repeated start
	set_addr(PCS_READ_ADDR); 
	uint8_t ans = read_nack();
	stop_condition(); 
	return ans; 
}

int main(void)
{
	// LCD
	LCD_inicialization();
	//LCD_WR_string("(000, 000, 000)"); 
	
	// PWM
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(0<<CS02)|(0<<CS01)|(1<<CS00);
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
	ICR1=255;
	
	DDRB|=(1<<3);
	DDRD|=(1<<5)|(1<<4);
	
	OCR0=255;
	OCR1A=255;
	OCR1B=255;
	
	// I2C - TCS34725
	PORTC = 255; 
	//I2C_Init(); 
	//start_condition(); 
	//_delay_ms(1000); 
	//LCD_CMD_8BIT(LCD_Cmd_Clear); 
	//set_addr(PCS_WRITE_ADDR); 
	//_delay_ms(1000); 
	//LCD_CMD_8BIT(LCD_Cmd_Clear); 
	//write_data(0x92);
	//_delay_ms(1000);
	//LCD_CMD_8BIT(LCD_Cmd_Clear);
	//start_condition(); 
	//_delay_ms(1000);
	//LCD_CMD_8BIT(LCD_Cmd_Clear);
	//set_addr(PCS_READ_ADDR); 
	//_delay_ms(1000);
	//LCD_CMD_8BIT(LCD_Cmd_Clear);	
	//TWCR=(1<<TWEN)|(1<<TWINT);								/* Enable TWI and clear interrupt flag */
	//while (!(TWCR & (1<<TWINT)));							/* Wait until TWI finish its current job (read operation) */
	//show_number(TWDR);
	//_delay_ms(1000); 
	//_delay_ms(1000); 
	//stop_condition();  
	show_number(read_cmd_8(0x12)); 
	//uint8_t x = read8(0x12); 
	////LCD_CMD_8BIT(LCD_Cmd_Clear);
	//LCD_WR_string("ID: ");  
	//show_number(x); 
	
	while (1)
	{
		//for(int i = 0; i < 256; i+=16) {
			//for(int j = 0; j < 256; j+=16) {
				//for(int k = 0; k < 256; k+=16) {
					//show_rgb(i, j, k); 
					//OCR0 = i; 
					//OCR1A = j; 
					//OCR1B = k; 
				//}
			//}
		//}
	}
}

