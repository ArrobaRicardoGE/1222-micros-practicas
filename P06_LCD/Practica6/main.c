/*
 * Practica6.c
 *
 * Created: 17/02/2022 03:22:03 p. m.
 * Author : ricar
 */ 
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "lcd.h"

int main(void)
{
	LCD_inicialization();
	LCD_CMD_8BIT(0b10000000);
	LCD_WR_string("Hola Mundo!");
	/* Replace with your application code */
	while (1)
	{
	}
}