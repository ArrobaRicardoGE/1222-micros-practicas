/*
 * P12_ADCEEPROM.c
 *
 * Created: 21/03/2022 01:41:06 p. m.
 * Author : ricar
 */ 

#define ADC_vect _VECTOR(14)
#define TIMER0_COMP_vect _VECTOR(19)
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "kbd.h"
#include "EEPROM.h"

volatile uint16_t next_dir = 0, current_dir; 
volatile int measuring = 1; 

void to_string(uint16_t x, char* s, int offset) {
	s[offset + 2] = x % 10 + '0';
	x /= 10;
	s[offset + 1] = x % 10 + '0';
	x /= 10;
	s[offset] = x % 10 + '0';
}

ISR(ADC_vect) {
	if(!measuring) return;	
	if(next_dir == 512) {
		measuring = 0; 
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("EEPROM llena"); 
		return; 
	}
	EEPROM_write(next_dir++, ADCH, 1);
}
ISR (TIMER0_COMP_vect) {}


int main(void)
{
	LCD_inicialization(); 
	config_kbd(); 
	
	sei();
	
	// Configurar ADC 
	ADMUX = 0b01100000; // 8 bits - ADCH
	SFIOR = 0b01100000; 
	ADCSRA = 0b10111011;
	
	// Configurar Timer 0
	OCR0 = 243;
	TCCR0 = 0b00001101; // prescaler 1024
	TIMSK = 0b00000010; 
	
	while (1)
	{
		measuring = 1; 
		next_dir = 0; 
		LCD_CMD_8BIT(LCD_Cmd_Clear); 
		LCD_WR_string("Sensando...");
		int8_t r = -1; 
		while(r != 1)
			r = read_kbd(); 
		measuring = 0; 
		LCD_CMD_8BIT(LCD_Cmd_Clear); 
		LCD_WR_string("Ultimos val:"); 
		LCD_CMD_8BIT(LCD_Cmd_endl); 
		current_dir = next_dir - 1; 
		while(1) {
			char s[10] = "#000: 000";
			to_string(current_dir, s, 1); 
			to_string(EEPROM_read(current_dir), s, 6);  
			LCD_WR_string(s);
			r = -1; 
			while(r == -1)
				r = read_kbd(); 
			if(r == 1) break;
			if(r == 2) current_dir = (current_dir == 0 ? 0 : current_dir - 1);
			if(r == 3) current_dir = (current_dir == next_dir - 1 ? current_dir : current_dir + 1);
			for(int i = 0; i < 9; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL); 
		}
	}
}

