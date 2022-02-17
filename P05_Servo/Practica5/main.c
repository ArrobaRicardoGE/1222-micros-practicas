/*
 * Practica5.c
 *
 * Created: 10/02/2022 03:18:30 p. m.
 * Author : ricar
 */ 
#define F_CPU 4000000

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

volatile uint8_t pos = 0;

uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (!(*LUGAR&(1<<BIT)));
}

void change_pos() {
	switch(PORTD) {
		case 0:
			OCR0 = 35;
			break;
		case 1:
			OCR0 = 48;
			break;
		case 2:
			OCR0 = 61;
			break;
		case 3:
			OCR0 = 74;
			break;
		case 4:
			OCR0 = 88;
			break;
		case 5:
			OCR0 = 101;
			break;
		case 6:
			OCR0 = 114;
			break;
		case 7:
			OCR0 = 127;
			break;
		case 8:
			OCR0 = 138;
			break;
		default:
			OCR0 = 148;
	}
}

void read_kbd() {
	// enter
	PORTA = 0b11111110;
	_delay_ms(50);
	if(cero_en_bit(&PINA, 4)) 
		change_pos();
	while(cero_en_bit(&PINA, 4));
	
	// down
	PORTA = 0b11110111;
	_delay_ms(50);
	if(cero_en_bit(&PINA, 5))
		PORTD = (PORTD == 0 ? 0 : PORTD - 1);
	while(cero_en_bit(&PINA, 5));
	
	// up
	PORTA = 0b11111101;
	_delay_ms(50);
	if(cero_en_bit(&PINA, 7))
		PORTD = (PORTD == 9 ? 9 : PORTD + 1); 
	while(cero_en_bit(&PINA, 7));
	
}


int main(void)
{
	// Puertos
    DDRA = 0b00001111;
	PORTA = 255;
	DDRB = DDRD = 255;
	
	// PWM
	TCCR0 = 0b01101011; // prescaler 64, fastpwm, inicia en 1
	OCR0 = 35; // iniciar en 0
    while (1) 
    { 
		read_kbd();
    }
}

