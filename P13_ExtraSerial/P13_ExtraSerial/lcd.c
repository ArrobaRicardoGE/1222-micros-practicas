#include "lcd.h"

void LCD_WR_string(volatile char *s){
	char c;
	while((c=*s++)){
		LCD_WR_CHAR(c);
	}
}

void LCD_inicialization(void){
	DDRLCD = 0b01111111; //0b0111_1111;
	//Basic LCD INIT
	_delay_ms(16);
	LCD_CMD_4BIT(0b00000011);
	_delay_ms(4.2);
	LCD_CMD_4BIT(0b00000011);
	_delay_us(100);
	LCD_CMD_4BIT(0b00000011);
	_delay_us(100);
	LCD_CMD_4BIT(0b00000010);
	_delay_us(100);
	//Full LCD INIT
	LCD_CMD_8BIT(LCD_Cmd_Func2Lin); //4 Bits, número de líneas y tipo de letra
	LCD_CMD_8BIT(LCD_Cmd_Off); //apaga el display
	LCD_CMD_8BIT(LCD_Cmd_Clear); //limpia el display
	LCD_CMD_8BIT(LCD_Cmd_ModeDnS); //Entry mode set ID S
	LCD_CMD_8BIT(LCD_Cmd_OnsCsB); //Enciende el display
	LCD_CMD_8BIT(0b10000000); //Write data
}
void LCD_WR_CHAR(uint8_t dato){
	PORTLCD = dato>>4;
	saca_uno(&PORTLCD, RS);
	saca_cero(&PORTLCD, RW);
	saca_uno(&PORTLCD, E);
	_delay_ms(10);
	saca_cero(&PORTLCD, E);
	PORTLCD = dato & 0b00001111;
	saca_uno(&PORTLCD, RS);
	saca_cero(&PORTLCD, RW);
	saca_uno(&PORTLCD, E);
	_delay_ms(10);
	saca_cero(&PORTLCD, E);
	busy_flag();
}
void LCD_CMD_4BIT(uint8_t instruction){
	PORTLCD=instruction;
	saca_cero(&PORTLCD, RS);
	saca_cero(&PORTLCD, RW);
	saca_uno(&PORTLCD, E);
	_delay_ms(10);
	saca_cero(&PORTLCD, E);
	busy_flag();
}
void LCD_CMD_8BIT(uint8_t instruction){
	// 0bxxxx_yyyy = VAR;
	// var = var >> 4;
	//var = 0b0000_xxxx;
	
	//Parte más significativa
	PORTLCD = instruction >> 4;
	saca_cero(&PORTLCD, RS);
	saca_cero(&PORTLCD, RW);
	saca_uno(&PORTLCD, E);
	_delay_ms(10);
	saca_cero(&PORTLCD, E);
	
	//Parte menos significativa
	PORTLCD = instruction & 0b00001111;
	saca_cero(&PORTLCD, RS);
	saca_cero(&PORTLCD, RW);
	saca_uno(&PORTLCD, E);
	_delay_ms(10);
	saca_cero(&PORTLCD, E);
	busy_flag();
}

void saca_uno(volatile uint8_t *LUGAR, uint8_t BIT){
	*LUGAR=*LUGAR|(1<<BIT);
}

void saca_cero(volatile uint8_t *LUGAR, uint8_t BIT){
	*LUGAR=*LUGAR&~(1<<BIT);
}
void busy_flag(void){
	DDRLCD&=0b11110000; //Para poner el pin BF como entrada para leer la bandera lo demás salida
	saca_cero(&PORTLCD,RS);// Instrucción
	saca_uno(&PORTLCD,RW); // Leer
	while(1){
		saca_uno(&PORTLCD,E); //pregunto por el primer nibble
		_delay_ms(10);
		saca_cero(&PORTLCD,E);
		if(uno_en_bit(&PINLCD,BF)) {break;} //uno_en_bit para proteus, 0 para la vida real
		_delay_us(10);
		saca_uno(&PORTLCD,E); //pregunto por el segundo nibble
		_delay_ms(10);
		saca_cero(&PORTLCD,E);
	}
	saca_uno(&PORTLCD,E); //pregunto por el segundo nibble
	_delay_ms(10);
	saca_cero(&PORTLCD,E);
	//entonces cuando tenga cero puede continuar con esto...
	saca_cero(&PORTLCD,RS);
	saca_cero(&PORTLCD,RW);
	DDRLCD|=(15<<0)|(1<<RS)|(1<<RW)|(1<<E);// 0b0111_1111
}