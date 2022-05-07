#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Includes
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "util.h"

// Defines
#define DDRLCD DDRB
#define PORTLCD PORTB	
#define PINLCD PINB
#define RS 4
#define RW 5
#define E 6
#define BF 3
#define LCD_Cmd_Clear 0b00000001
#define LCD_Cmd_Home 0b00000010
#define LCD_Cmd_Func2Lin 0b00101000
#define LCD_Cmd_Off	0b00001000
#define LCD_Cmd_ModeDnS	 0b00000110
#define LCD_Cmd_OnsCsB	0b00001100		// Entry mode / cursor off
#define LCD_Cmd_Blink	0b00001111		// Blinking on / cursor on
#define LCD_Cmd_endl 0b11000000
#define LCD_Cmd_ShiftL 0b00010000

// Prototypes
void LCD_inicialization(void);
void LCD_CMD_8BIT(uint8_t instruction);
void LCD_CMD_4BIT(uint8_t instruction);
void LCD_WR_CHAR(uint8_t dato);
void saca_uno(volatile uint8_t *LUGAR, uint8_t BIT);
void saca_cero(volatile uint8_t *LUGAR, uint8_t BIT);
void busy_flag(void);
void LCD_WR_string(volatile char *s);

#endif