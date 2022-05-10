/*
 * Final.c
 *
 * Created: 08/05/2022 04:25:50 p. m.
 * Author : Ricardo Gutierrez
 */ 

#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "util.h"

#define MOSI 5
#define SCK 7
#define MISO 6
#define SS 4
#define RESET 0
#define PORTSS PORTA
#define DDRSS DDRA
#define PORTCSCD PORTD
#define DDRCSCD DDRD
#define CS 0
#define CD 1

 void transmit(uint8_t cData, uint8_t Device)
 {
	 /* Start transmission */
	 PORTSS = ~(1<<Device); // Pongo un 0 en el bit de SS
	 SPDR = cData; //Comiendo el envío de información
	 /* Wait for transmission complete */
	 while(!(SPSR & (1<<SPIF)));
	 PORTSS = 255;
 }

 void SPI_Write(uint8_t data) {
	 transmit(data, 4); // screen on 4
 }

#define CS_IDLE     saca_uno(&PORTCSCD, CS)
#define RD_IDLE     0
#define WR_IDLE     0
#define CS_ACTIVE   saca_cero(&PORTCSCD, CS)
#define CD_COMMAND  saca_cero(&PORTCSCD, CD)
#define CD_DATA     saca_uno(&PORTCSCD, CD)

#define write8(d) SPI_Write(d)
#define WR_STROBE { }
#define write16(d) write8(d>>8); write8(d)
#define writeData16(x)  CD_DATA; write16(x)
#define writeCmd16(x)  CD_COMMAND; write16(x)
#define writeCmdData16(a, d)  CD_COMMAND; write8(a>>8); write8(a); CD_DATA; write8(d>>8); write8(d)

#define ILI932X_HOR_START_AD       0x50
#define ILI932X_HOR_END_AD         0x51
#define ILI932X_VER_START_AD       0x52
#define ILI932X_VER_END_AD         0x53
#define ILI932X_GRAM_HOR_AD        0x20
#define ILI932X_GRAM_VER_AD        0x21

#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0

void reset() {
	CS_IDLE;
	RD_IDLE;
	WR_IDLE;
	saca_uno(&PORTB, RESET);
	CS_ACTIVE;
	CD_COMMAND;
	write8(0x00);
	for(uint8_t i=0; i<3; i++)
	{
		WR_STROBE; // Three extra 0x00s
	}
	CS_IDLE;
}

void start() {
	reset();
	_delay_ms(200);
}

void Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
	CS_ACTIVE;
	writeCmdData16(ILI932X_HOR_START_AD, x1); // Set address window
	writeCmdData16(ILI932X_HOR_END_AD, x2);
	writeCmdData16(ILI932X_VER_START_AD, y1);
	writeCmdData16(ILI932X_VER_END_AD, y2);
	writeCmdData16(ILI932X_GRAM_HOR_AD, x1 ); // Set address counter to top left
	writeCmdData16(ILI932X_GRAM_VER_AD, y1 );
	CS_IDLE;
}

void draw_pixel(int16_t x, int16_t y, uint16_t color) {
	Set_Addr_Window(x, y, x, y);
	CS_ACTIVE;
	writeData16(color);
}

void init_lcd() {
	reset();
	start();
}


int main(void)
{
	DDRB |= (1 << MOSI)|(1 << SCK);
	SPCR |= (1 << SPE)|(1 << MSTR);
	DDRSS = 255;
	PORTSS = 255;
	DDRCSCD = 255; 
	PORTCSCD = 255; 
    
	init_lcd();
	
    while (1) 
    {
		reset(); 
		for(int i = 0; i < 240; i++) {
			for(int j = 0; j < 320; j++) {
				draw_pixel(i, j, BLUE); 
				_delay_ms(50); 
			}
		}
    }
}

