/*
 * Follower.c
 *
 * Created: 07/05/2022 04:13:12 p. m.
 * Author : ricar
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

#define MOSI 5
#define SCK 7
#define MISO 6
#define SS 4

uint8_t receive()
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return data register */
	return SPDR;
}

int main(void)
{
    PORTB |= (1 << MISO); 
	DDRC = 255;
	SPCR |= (1 << SPE);
	
    while (1) 
    {
		uint8_t dato = receive(); 
		PORTC = dato; 
		_delay_ms(1000); 
		PORTC = 0; 
    }
}

