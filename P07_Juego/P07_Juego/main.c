/*
 * P07_Juego.c
 *
 * Created: 22/02/2022 03:21:48 p. m.
 * Author : ricar
 */ 

#include <avr/io.h>
#include "kbd.h"


int main(void)
{
    /* Replace with your application code */
	config_kbd();
	DDRB = 255;
	
    while (1) 
    {
		int8_t r = read_kbd();
		PORTB = r;  
    }
}

