/*
 * Leader.c
 *
 * Created: 03/05/2022 08:50:56 p. m.
 * Author : ricar
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

#define MOSI 5
#define SCK 7
#define MISO 6
#define SS 4
#define PORTSS PORTA
#define DDRSS DDRA

 void transmit(uint8_t cData, uint8_t Device)
 {
	 /* Start transmission */
	 PORTSS = ~(1<<Device); // Pongo un 0 en el bit de SS
	 SPDR = cData; //Comiendo el envío de información
	 /* Wait for transmission complete */
	 while(!(SPSR & (1<<SPIF)));
	 PORTSS = 255; 
 }

void send_data() {
	transmit(0b11111110, 6);
	transmit(0b11110000, 7);
}

int main(void)
{
    DDRB |= (1 << MOSI)|(1 << SCK);
	PORTC = 255; 	
	SPCR |= (1 << SPE)|(1 << MSTR); 
	DDRSS = 255; 
	PORTSS = 255; 
	
	while (1) 
    {
		if(!(PINC & 1)) {
			send_data(); 
			while(!(PINC & 1)); 
		}
    }
}

