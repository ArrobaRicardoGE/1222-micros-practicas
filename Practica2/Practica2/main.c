/*
 * Practica2.c
 *
 * Created: 01/02/2022 04:00:35 p. m.
 * Author : ricar
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

int8_t val = 0;

void sumar() {
	val++;
	if(val == 100) val = 99;
}

void restar() {
	val--;
	if(val == -1) val = 0;
}

void show() {
	uint8_t u = val % 10;
	uint8_t d = val / 10;
	PORTD = (d + u * 16);
}

int main(void){
	DDRA = 0;
	PORTA = 255;
	DDRD = 255;
	PORTD = 0;
    while (1){
		if(!(PINA & 1)){
			_delay_ms(50);
			if(PINA & 1) continue;
			sumar();
			while (!(PINA & 1));
			_delay_ms(50);
		}
		if(!(PINA & (1<<7))){
			_delay_ms(50);
			if(PINA & (1<<7)) continue;
			restar();
			while (!(PINA & (1<<7)));
			_delay_ms(50);
		}
		show(); 
    }
}

