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

int main(void)
{
    DDRB |= (1 << MOSI)|(1 << SCK);
	PORTC = 255; 	
	
	while (1) 
    {
		if(!(PINC & 1)) {
			send_data(); 
		}
    }
}

