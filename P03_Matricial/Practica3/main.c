/*
 * Practica3.c
 *
 * Created: 03/02/2022 04:26:08 p. m.
 * Author : ricar
 */ 

#define F_CPU 1000000 

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

uint8_t vals[5];

uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT)
{
	return (!(*LUGAR&(1<<BIT)));
}

void show_digits() {
	PORTB = (vals[1] << 4) + vals[0];
	PORTD = (vals[3] << 4) + vals[2];
}

void register_value(uint8_t val) {
	if(val == 10){
		for(int i = 0; i < 4; i++)
			vals[i] = 0;
	} else {
		for(int i = 4; i > 0; i--)
			vals[i] = vals[i - 1];
		vals[0] = val;
	}
	show_digits();
}

void read_kbd() {
	// primera columna
	PORTA = 0b11111110;
	_delay_ms(50); 
	for(uint8_t i = 4; i <= 7; i++ ){
		if(cero_en_bit(&PINA, i)) {
			register_value(10);
			while(cero_en_bit(&PINA, i));
		}
	}
	// segunda columna
	PORTA = 0b11111101;
	_delay_ms(50);
	for(uint8_t i = 4; i <= 7; i++ ){
		uint8_t val = (i - 4) * 3;
		if(!val) val = 10;
		if(cero_en_bit(&PINA, i)) {
			register_value(val);
			while(cero_en_bit(&PINA, i));
		}
	}
	// tercera columna
	PORTA = 0b11111011;
	_delay_ms(50);
	for(uint8_t i = 4; i <= 7; i++ ){
		uint8_t val = (i - 4) * 3;
		if(val) val--;
		if(cero_en_bit(&PINA, i)) {
			register_value(val);
			while(cero_en_bit(&PINA, i));
		}
	}
	// cuarta columna
	PORTA = 0b11110111;
	_delay_ms(50);
	for(uint8_t i = 4; i <= 7; i++ ){
		uint8_t val = (i - 4) * 3;
		if(!val) val = 10;
		else val -= 2; 
		if(cero_en_bit(&PINA, i)) {
			register_value(val);
			while(cero_en_bit(&PINA, i));
		}
	}
}

int main(void)
{
    DDRA = 0b00001111;
	PORTA = 255;
	DDRB = DDRD = 255;
    while (1) 
    {
		read_kbd();
    }
}

