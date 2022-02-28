/*
 * P07_Juego.c
 *
 * Created: 22/02/2022 03:21:48 p. m.
 * Author : ricar
 */ 

#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "kbd.h"
#include "lcd.h"

volatile uint8_t aciertos, intentos; 
volatile uint8_t to_guess[3][12];     

int main(void)
{
	config_kbd();
	LCD_inicialization();
	DDRB = 255;
	
    while (1) 
    {	
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Cuantas");
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_string("coordenadas?");
		int8_t r = 10;
		while(r == 10) 
			r = read_kbd();
			
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Escondere ");
		LCD_WR_CHAR(r + '0');
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_string("coordenadas");
		_delay_ms(2000);
		
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Tu debes buscar");
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_CHAR(r + '0');
		LCD_WR_string(" coordenadas.");
		_delay_ms(2000);
		
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("2 filas (0,1)");
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_string("10 cols (0-9)");
		_delay_ms(2000);
		
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Presiona +");
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_string("para continuar..");
		
		intentos = r * 2;
		while(r != 10)
			r = read_kbd();
		
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Trata de");
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_string("memorizar...");
		_delay_ms(2000);
		
		srand(intentos); 
		int to_select = 0;
		while(to_select < intentos / 2){
			int x = rand() % 2;
			int y = rand() % 10; 
			if(!to_guess[x][y]) {
				to_guess[x][y] = 1;
				LCD_CMD_8BIT(LCD_Cmd_Clear);
				LCD_WR_string("Cuadrito ");
				LCD_WR_CHAR(to_select + '1');
				LCD_CMD_8BIT(0b11000000); // salto de linea
				LCD_WR_CHAR(x + '0');
				LCD_WR_CHAR(',');
				LCD_WR_CHAR(y + '0');
				_delay_ms(700);
				to_select++; 
			}
		}
		
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Tienes ");
		LCD_WR_CHAR('0' + intentos / 10); 
		LCD_WR_CHAR('0' + intentos % 10);
		LCD_CMD_8BIT(0b11000000); // salto de linea
		LCD_WR_string("intentos");
		_delay_ms(2000);
		
		for(int8_t i = 1; i <= intentos; i++) {
			LCD_CMD_8BIT(LCD_Cmd_Clear);
			LCD_WR_string("Intento ");
			LCD_WR_CHAR('0' + i / 10);
			LCD_WR_CHAR('0' + i % 10);
			int8_t x = read_kbd();
			LCD_WR_string(" (");
			LCD_WR_CHAR(x + '0');
			LCD_WR_CHAR(',');
			int8_t y = read_kbd();
			LCD_WR_CHAR(y + '0');
			LCD_WR_CHAR(')');
			LCD_CMD_8BIT(0b11000000); // salto de linea
			if(x > 1) {
				LCD_WR_string("No existe lugar");
				_delay_ms(1000); 
			}
			else if(to_guess[x][y] == 1) {
				LCD_WR_string("Acierto");
				aciertos++; 
				PORTB = aciertos;
				to_guess[x][y] = 2;
				_delay_ms(1000); 
			}
			else if(to_guess[x][y] == 0){
				LCD_WR_string("Error");
				_delay_ms(1000); 
			}
			else {
				LCD_WR_string("Ya estaba!");
				i--; 
				_delay_ms(1000); 
			}
			
			if(aciertos == intentos / 2) break;
				
		}
		
		if(aciertos == intentos / 2) {
			LCD_CMD_8BIT(LCD_Cmd_Clear);
			LCD_WR_string("Tienes excelente");
			LCD_CMD_8BIT(0b11000000); // salto de linea
			LCD_WR_string("memoria!");
		} else {
			LCD_CMD_8BIT(LCD_Cmd_Clear);
			LCD_WR_string("Tu memoria no es");
			LCD_CMD_8BIT(0b11000000); // salto de linea
			LCD_WR_string("muy buena :(");
		}
		_delay_ms(4000);
		
		aciertos = 0;
		intentos = 0;
		PORTB = 0;
		for(int i = 0; i < 2; i++)
			for(int j = 0; j < 10; j++)
				to_guess[i][j] = 0;
		LCD_CMD_8BIT(LCD_Cmd_Clear);
    }
}

