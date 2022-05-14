/*
 * Final.c
 *
 * Created: 08/05/2022 04:25:50 p. m.
 * Author : Ricardo Gutierrez
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "util.h"
#include "lcd.h"

void show_number(uint8_t x, uint8_t ret) {
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
	show_number(r, 14);  
	for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
	show_number(g, 9);
	for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
	show_number(b, 4);
	for(int i = 0; i < 1; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
}

int main(void)
{
	LCD_inicialization();
	LCD_WR_string("(000, 000, 000)"); 
	_delay_ms(1000);
	
	// PWM
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(0<<CS02)|(0<<CS01)|(1<<CS00);
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
	ICR1=255;
	
	DDRB|=(1<<3);
	DDRD|=(1<<5)|(1<<4);
	
	OCR0=0;
	OCR1A=0;
	OCR1B=0;
	
	while (1)
	{
		for(int i = 0; i < 256; i+=16) {
			for(int j = 0; j < 256; j+=16) {
				for(int k = 0; k < 256; k+=16) {
					show_rgb(i, j, k); 
					OCR0 = i; 
					OCR1A = j; 
					OCR1B = k; 
				}
			}
		}
		
	}
}

