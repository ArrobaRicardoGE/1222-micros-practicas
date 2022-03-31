/*
 * P14_VoltimetroPorSerial.c
 *
 * Created: 31/03/2022 03:36:01 p. m.
 * Author : ricar
 */ 

#define F_CPU 1000000
#define BAUD 4800
#define MYUBRR F_CPU/16/BAUD-1
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void saca_uno(volatile uint8_t *LUGAR, uint8_t BIT){
	*LUGAR=*LUGAR|(1<<BIT);
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT){
	return ((*LUGAR&(1<<BIT)));
}

uint8_t lectura_adc(uint8_t adcbit) {
	ADMUX &= 0b11100000;
	ADMUX |= adcbit;
	saca_uno(&ADCSRA, 6);
	while(uno_en_bit(&ADCSRA, 6));
	return ADCH;
}

void mandar_serial(uint8_t dato) {
	UDR = dato; 
	while(!uno_en_bit(&UCSRA, 6)); 
	_delay_ms(1); 
}

int main(void)
{	
	// ADC
	ADMUX = 0b01100000;
	SFIOR = 0;
	ADCSRA = 0b10010100;
	
	// Serial
	uint8_t ubrr = MYUBRR;
	DDRD |= (1<<1);        //TX de salida (D1) y RX de entrada (D0)
	UBRRH = (uint8_t) (ubrr>>8);
	UBRRL = (uint8_t) (ubrr);
	UCSRB = (1<<TXEN);
	//habilité transmisión
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0)|(1<<USBS);
	//paquetes de 8 bits, con 2 bits de parada, sin paridad
	
	uint8_t r = lectura_adc(2), c; 
	mandar_serial(r);
	while (1)
	{
		c = r; 
		r = lectura_adc(2); 
		if(r != c) mandar_serial(r); 
		
	}
}
