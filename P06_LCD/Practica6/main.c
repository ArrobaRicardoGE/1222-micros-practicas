/*
 * Practica6.c
 *
 * Created: 17/02/2022 03:22:03 p. m.
 * Author : ricar
 */ 
#define F_CPU 1000000
#define TIMER0_COMP_vect _VECTOR(19)

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "lcd.h"

volatile uint8_t cnt = 0;
volatile uint8_t letters = 0;

ISR(TIMER0_COMP_vect) {
	cnt++;
}

void write_letter() {
	LCD_CMD_8BIT(LCD_Cmd_OnsCsB); // apagar blink
	cnt = 0;
	char c = 'A';
	LCD_WR_CHAR(c);
	_delay_ms(50);
	while(cero_en_bit(&PINA, 0)){
		cnt = 0; 
	}; 
	while(cnt < 100) {
		if(cero_en_bit(&PINA, 0)){
			cnt = 0;
			c = (c == 'Z' ? 'A' : c + 1); 
			LCD_CMD_8BIT(0b00010000); // shift to left 
			LCD_WR_CHAR(c);
			_delay_ms(50);
			while(cero_en_bit(&PINA, 0)){
				cnt = 0; 
			}; 
		}
	}
	letters++;
	LCD_CMD_8BIT(LCD_Cmd_Blink); // prender blink
}

int main(void)
{
	
	// Timer
	TCCR0 = 0b00001011; // CTC, prescaler 64
	TIMSK = 0b00000010; // activar comp
	OCR0 = 155;
	sei();
	
	// Puertos
	DDRA = 0;
	PORTA = 1;
	
	//LCD_inicialization();
	//LCD_CMD_8BIT(0b10000000); //Write data
	//LCD_CMD_8BIT(LCD_Cmd_OnCrs);
	//LCD_WR_string("Hola Mundo!");
	//LCD_WR_string("xd");
	//LCD_CMD_8BIT(LCD_Cmd_OnsCsB);
	
	LCD_inicialization();
	LCD_CMD_8BIT(0b10000000); //Write data
	LCD_CMD_8BIT(LCD_Cmd_Blink);
	
	while (1)
	{
		if(cero_en_bit(&PINA, 0)) {
			if(letters == 16){
				LCD_CMD_8BIT(LCD_Cmd_Clear);
				letters = 0;
				while(cero_en_bit(&PINA, 0)); 
			}
			else
				write_letter();
		}
	}
}