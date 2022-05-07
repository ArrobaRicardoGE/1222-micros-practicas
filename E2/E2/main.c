/*
 * E2.c
 *
 * Created: 05/04/2022 02:03:10 p. m.
 * Author : Ricardo Antonio Gutierrez Esparza
 */ 
#define F_CPU 8000000
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "kbd.h"
#include "lcd.h"
#include "EEPROM.h"

volatile uint8_t enable_serial_rx, dato_serial_rx, f_serial_rx; 

ISR(USART_RXC_vect) {
	dato_serial_rx = UDR;
	if(!enable_serial_rx) return;
	f_serial_rx = 1;  
}

uint16_t read_adc() {
	saca_uno(&ADCSRA, 6);
	while(uno_en_bit(&ADCSRA, 6));
	uint16_t c = ADC;
	return c; 
}

uint8_t read_serial() {
	f_serial_rx = 0; 
	enable_serial_rx = 1; 
	while(!f_serial_rx); 
	return dato_serial_rx; 
}

void write_serial(uint8_t dato) {
	while(cero_en_bit(&UCSRA, TXC) && cero_en_bit(&UCSRA, UDRE));
	UDR = dato; 
}

uint8_t map_v(double v) {
	if(v < 1) return 1; 
	if(v < 2) return 2; 
	if(v < 3) return 3; 
	if(v < 4) return 4; 
	return 5; 	
}

uint8_t valido(uint8_t dato) {
	return (dato <= '9' && dato >= '0');
}

int main(void)
{
	// Inicar componentes
    LCD_inicialization();
	config_kbd();
	
	// Configurar ADC
	ADMUX = 0b01000111; // AVCC ref, 10 bits, ADC7
	SFIOR = 0; // free running
	ADCSRA = 0b10010111; // div 128
	
	// Configurar serial
	sei(); 
	uint8_t ubrr = MYUBRR;
	DDRD |= (1<<1);
	UBRRH = (uint8_t) (ubrr>>8);
	UBRRL = (uint8_t) (ubrr);
	UCSRB = (1<<TXEN)|(1<<RXEN)|(1<<RXCIE); // tx, rx, interrupcion de rx
	UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); // 8 bits, con 1 bits de parada, sin paridad
	
    while (1) 
    {
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Esperando C"); 
		while(read_serial() != 'C');
		
		uint8_t n = map_v(read_adc() * 5.0 / 1023.0); 
		LCD_CMD_8BIT(LCD_Cmd_Clear); 
		LCD_WR_string("Esperando "); 
		LCD_WR_CHAR(n + '0'); 
		LCD_WR_string(" val.");
		LCD_CMD_8BIT(LCD_Cmd_endl);
		
		for(int i = 0; i < n; i++) {
			uint8_t rx = 'x'; 
			while(!valido(rx)) rx = read_serial();
			LCD_WR_CHAR(rx);
			EEPROM_write((uint16_t)i, rx, 1); 
			LCD_WR_CHAR(' '); 
		}
		for(int i = n; i <= 4; i++)
			EEPROM_write((uint16_t)i, '@', 1); 
		write_serial('X'); 	
		
		LCD_WR_CHAR('.');
		_delay_ms(1000);
		
		LCD_CMD_8BIT(LCD_Cmd_Clear); 
		LCD_WR_string("Preparando"); 
		LCD_CMD_8BIT(LCD_Cmd_endl);
		LCD_WR_string("para comenzar"); 
		_delay_ms(500);
		
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Escribe...");
		LCD_CMD_8BIT(LCD_Cmd_endl);
		
		for(int i = 0; i < n; i++) {
			uint8_t expected = EEPROM_read((uint16_t)i);
			int8_t r = -1; 
			while(r != expected - '0') r = read_kbd();
			LCD_WR_CHAR(r + '0');
			LCD_WR_CHAR(' ');
		}
		
		_delay_ms(500);
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Terminado");
		_delay_ms(1000);
    }
}

