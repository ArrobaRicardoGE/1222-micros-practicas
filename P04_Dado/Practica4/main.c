/*
 * Practica4.c
 *
 * Created: 08/02/2022 03:20:39 p. m.
 * Author : ricar
 */ 

#define  F_CPU 1000000
#define INT0_vect _VECTOR(1)

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int8_t dado = 1;

ISR(INT0_vect) {
	switch(dado) {
		case 1:
			PORTA = 0b00010000;
			PORTB = 0;
			break;
		case 2:
			PORTA = 0b01000100;
			PORTB = 0;
			break;
		case 3:
			PORTA = 0b00010001;
			PORTB = 1;
			break;
		case 4:
			PORTA = 0b01000101;
			PORTB = 1;
			break;
		case 5:
			PORTA = 0b01010101;
			PORTB = 1;
			break;
		default:
			PORTA = 0b01101101;
			PORTB = 1;
	}
	while(!(PIND & (1 << 2)));
	_delay_ms(50);
}


int main(void)
{
    // Habilitar interrupciones
	sei();
	MCUCR |= 2;			// flanco de bajada
	GIFR |= (1 << 6);	// limpiar bandera
	GICR |= (1 << 6);	// activar INT0
	
	// Configurar puertos 
	DDRA = 255;
	DDRB = 1;
	DDRD = 0;
    while (1) 
    {
		dado = (dado == 6 ? 1 : dado + 1);
    }
}

