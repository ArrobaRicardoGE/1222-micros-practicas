#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Includes
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "util.h"

// Defines
#define DDRLCD DDRA
#define PORTLCD PORTA
#define PINLCD PINA
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
#define LCD_Cmd_Blink	0b00001111
#define LCD_Cmd_ShiftL 0b00010000
#define LCD_Cmd_ShiftR 0b00010100

// Prototypes
void LCD_inicialization(void);
void LCD_CMD_8BIT(uint8_t instruction);
void LCD_CMD_4BIT(uint8_t instruction);
void LCD_WR_CHAR(uint8_t dato);
void busy_flag(void);
void LCD_WR_string(volatile char *s);

#endif