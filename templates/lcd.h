#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

// Defines
#define DDRLCD DDRD
#define PORTLCD PORTD
#define PINLCD PIND
#define RS 4
#define RW 5
#define E 6
#define BF 3
#define LCD_Cmd_Clear 0b00000001
#define LCD_Cmd_Home 0b00000010
#define LCD_Cmd_Func2Lin 0b00101000
#define LCD_Cmd_Off	0b00001000
#define LCD_Cmd_ModeDnS	 0b00000110
#define LCD_Cmd_OnsCsB	0b00001100

// Prototypes
void LCD_inicialization(void);
void LCD_CMD_8BIT(uint8_t instruction);
void LCD_CMD_4BIT(uint8_t instruction);
void LCD_WR_CHAR(uint8_t dato);
uint8_t cero_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT);
void saca_uno(volatile uint8_t *LUGAR, uint8_t BIT);
void saca_cero(volatile uint8_t *LUGAR, uint8_t BIT);
void busy_flag(void);
void LCD_WR_string(volatile char *s);

#endif