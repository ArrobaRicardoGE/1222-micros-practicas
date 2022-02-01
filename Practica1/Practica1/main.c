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
	DDRB = 0;
	PORTB = 255;
	DDRA = 255;
	PORTA = 0;
	while(1){
		valor = PINB;
		valor = ~valor;
		switch(valor){
			case 1:
				PORTA = 0b00000110;
				break;
			case 2:
				PORTA = 0b01011011;
				break;
			case 3:
				PORTA = 0b01001111;
				break;
			case 4:
				PORTA = 0b01100110;
				break;
			case 5:
				PORTA = 0b01101101;
				break;
			case 6:
				PORTA = 0b01111101;
				break;
			case 7:
				PORTA = 0b00000111;
				break;
			case 8:
				PORTA = 0b01111111;
				break;
			case 9:
				PORTA = 0b01101111;
				break;
			default:
				PORTA = 0b00111111;
		}
		_delay_ms(5);
    }
}

