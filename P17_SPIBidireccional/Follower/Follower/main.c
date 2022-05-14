/*
 * Follower.c
 *
 * Created: 12/05/2022 03:47:05 p. m.
 * Author : ricar
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

#define MOSI 5
#define SCK 7
#define MISO 6

//void transmit(uint8_t cData, uint8_t Device)
//{
	///* Start transmission */
	//PORTSS = ~(1<<Device); // Pongo un 0 en el bit de SS
	//SPDR = cData; //Comiendo el envío de información
	///* Wait for transmission complete */
	//while(!(SPSR & (1<<SPIF)));
	//PORTSS = 255;
//}

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
		uint8_t bit = receive(); 
		if(bit == 0) SPDR = 124;
		else SPDR = 233; 
		//receive(); // send
	}
}

