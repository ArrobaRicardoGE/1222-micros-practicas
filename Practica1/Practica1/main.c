/*
 * Practica1.c
 *
 * Created: 27/01/2022 03:34:07 p. m.
 * Author : ricar
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define F_CPU 1000000 // velocidad del procesador (1 Mhz)

int main(void){
    auto uint8_t valor = 0;
	DDRA = 0;
	PORTA = 255;
	DDRC = 255;
	PORTC = 0;
	while(1){
		valor = PINA;
		valor = ~valor;
		switch(valor){
			case 1:
				PORTC = 0b00000110;
				break;
			case 2:
				PORTC = 0b01011011;
				break;
			case 3:
				PORTC = 0b01001111;
				break;
			case 4:
				PORTC = 0b01100110;
				break;
			case 5:
				PORTC = 0b01101101;
				break;
			case 6:
				PORTC = 0b01111101;
				break;
			case 7:
				PORTC = 0b00000111;
				break;
			case 8:
				PORTC = 0b01111111;
				break;
			case 9:
				PORTC = 0b01101111;
				break;
			default:
				PORTC = 0b00111111;
		}
		_delay_ms(5);
    }
}

