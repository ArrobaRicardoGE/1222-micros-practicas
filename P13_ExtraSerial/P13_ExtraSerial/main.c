#define F_CPU 4000000
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
//#define  USART_RXC_vect 
#include <avr/io.h>
#include <stdint.h> //Para poder declarar varialbes especiales
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include "kbd.h"

void USART_Init (uint8_t ubrr)
{
	DDRD |= (1<<1);        //TX de salida (D1) y RX de entrada (D0)
	UBRRH = (uint8_t) (ubrr>>8);
	UBRRL = (uint8_t) (ubrr);
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	//habilité recepción, transmisión e interrupción de la recepción
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ2)|(0<<USBS);
	//paquetes de 8 bits, con 1 bits de parada, sin paridad
}
ISR(USART_RXC_vect)
{
	
	uint8_t dato = UDR;
	LCD_WR_CHAR(dato);
	PORTB = 128; 
}

int main(void)
{
	sei();
	LCD_inicialization();
	config_kbd(); 
	USART_Init(MYUBRR);
	DDRB = 255;
	while(1)
	{
		int8_t r = -1; 
		while(r == -1)
			r = read_kbd();
		//UDR = r;
		UDR = 'a';  
	}
}