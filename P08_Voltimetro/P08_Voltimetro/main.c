/*
 * P08_Voltimetro.c
 *
 * Created: 08/03/2022 11:31:40 a. m.
 * Author : ricar
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "util.h"
#include <stdint.h>

volatile uint16_t last_r = 0;

void update_lcd(double val) {
	char s[5];
	s[0] = (int)val + '0';
	s[1] = '.';
	s[2] = (int)(val * 10) % 10 + '0'; 
	s[3] = (int)(val * 100) % 10 + '0';
	s[4] = 0; 
	LCD_WR_string(s);
}

int main(void)
{
    LCD_inicialization();
	ADMUX = 0b01000111; // Pin 7, 10 bits, voltaje de AVCC
	SFIOR = 0; // No trigger
	ADCSRA = 0b10010011; // Factor de division 8
	LCD_WR_string("Voltaje");
	LCD_CMD_8BIT(0b11000000);
    while (1) 
    {
		saca_uno(&ADCSRA, 6);
		while(cero_en_bit(&ADCSRA, 6)); // esperar lectura
		uint16_t r = ADC; 
		if(r == last_r) continue;
		last_r = r; 
		double val = (double)r * 5.0 / 1024.0; 
		update_lcd(val);
		for(int i = 0; i < 4; i++)
			LCD_CMD_8BIT(0b00010000); 
    }
}

