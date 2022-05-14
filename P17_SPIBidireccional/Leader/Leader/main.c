/*
 * Leader.c
 *
 * Created: 12/05/2022 03:34:05 p. m.
 * Author : ricar
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "util.h"
#include "lcd.h"	

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

uint8_t receive()
{
	/* Wait for reception complete */
	while(!(SPSR & (1<<SPIF)));
	/* Return data register */
	return SPDR;
}

void show_number(uint8_t x) {
	int pad = 3;
	char ans[pad + 1];
	ans[pad--] = 0;
	while(x > 0) {
		ans[pad--] = (x % 10) + '0';
		x /= 10;
	}
	while(pad >= 0) {
		ans[pad--] = '0';
	}
	LCD_WR_string(ans);
}

int main(void)
{
	DDRB |= (1 << MOSI)|(1 << SCK);
	PORTC = 255;
	SPCR |= (1 << SPE)|(1 << MSTR);
	DDRSS = 255;
	PORTSS = 255;
	
	LCD_inicialization(); 
	
	LCD_WR_string("Hola"); 
	
	while (1)
	{
		if(cero_en_bit(&PINC, 0)) {
			while(cero_en_bit(&PINC, 0));
			transmit(0, 6); // wake up
			show_number(receive()); 
			_delay_ms(1000); 
			transmit(0, 6); // request
			LCD_CMD_8BIT(LCD_Cmd_Clear);
			show_number(receive());
		}
		if(cero_en_bit(&PINC, 2)) {
			transmit(1, 7); // wake up
			transmit(1, 7); // request
			while(cero_en_bit(&PINC, 2));
			LCD_CMD_8BIT(LCD_Cmd_Clear);
			show_number(receive());
		}
	}
}


