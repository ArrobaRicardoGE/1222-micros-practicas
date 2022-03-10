/*
 * P1.c
 *
 * Created: 01/03/2022 08:58:46 a. m.
 * Author : Ricardo Antonio Gutiérrez Esparza
 */ 

#define F_CPU 4000000
#define INT2_vect _VECTOR(18)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include "kbd.h"
#include "lcd.h"

volatile uint8_t letra, numero, cantidad; 
volatile uint8_t precios[2][2] = {{3, 9}, {23, 31}}; 
volatile uint8_t monedas[16] = {0,1,2,5,10,0,};
	
void write_price(uint8_t x, uint8_t zero_pad, uint8_t cents) {
	if(x / 10 || zero_pad) LCD_WR_CHAR(x / 10 + '0');
	LCD_WR_CHAR(x % 10 + '0');
	if(cents) LCD_WR_string(".00");
}

ISR(INT2_vect) {
	if(letra == 5 || numero == 10) return;
	uint8_t input = PINB >> 3;
	cantidad += monedas[input];
	if(monedas[input] != 0) {
		write_price(cantidad, 1, 0);
		for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL);
	}
}

int main(void)
{
	// Configurar periferales
    config_kbd();
	LCD_inicialization();
	DDRB = 1;
	PORTB = 0b01111100;
	
	// Configurar interrupcion
	sei();
	MCUCSR = 0;			// flanco de bajada
	GIFR |= (1 << 5);	// limpiar bandera
	GICR |= (1 << 5);	// activar INT2
	
    while (1) 
    {
		cantidad = 0; 
		letra = 5;
		numero = 10;
		// Mensaje inicial
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Elige Producto");
		LCD_CMD_8BIT(LCD_Cmd_endl);
		LCD_CMD_8BIT(LCD_Cmd_Blink);
		
		// Obtener letra
		while(letra < 10)
			letra = read_kbd();
		letra -= 10;
		LCD_WR_CHAR(letra + 'A'); 
		// Obtener numero
		while(numero > 9)
			numero = read_kbd();
		LCD_WR_CHAR(numero + '0');
		numero--;
		
		// Pantalla de precio
		LCD_CMD_8BIT(LCD_Cmd_OnsCsB);
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Inserta $");
		write_price(precios[letra][numero], 0, 1);
		LCD_CMD_8BIT(LCD_Cmd_endl);
		LCD_WR_string("Tienes $00.00");
		for(int i = 0; i < 5; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL);
		
		// Esperamos a recibir todo el dinero
		while(cantidad < precios[letra][numero]);
		
		// Entregamos
		uint8_t precio = precios[letra][numero];
		letra = 5; // Deshabilitar monedas
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Entregando...");
		LCD_CMD_8BIT(LCD_Cmd_endl);
		LCD_WR_string("Cambio ");
		write_price(cantidad - precio, 1, 1);
		PORTB |= 1; 
		_delay_ms(2000);
		PORTB ^= 1;
    }	
}

