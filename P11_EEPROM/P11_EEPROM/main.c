/*
 * P11_EEPROM.c
 *
 * Created: 17/03/2022 04:21:48 p. m.
 * Author : ricar
 */ 

#include <avr/io.h>
#include <stdint.h>
#include "lcd.h"
#include "kbd.h"

volatile uint16_t data_addr[4] = {10, 20, 30, 40}; 

void EEPROM_write(uint16_t dir, uint8_t dat){
	while (uno_en_bit(&EECR, EEWE));
	EEAR = dir;
	EEDR = dat;
	//cli();
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	//sei();
}

uint8_t EEPROM_read(uint16_t dir){
	while (uno_en_bit(&EECR, EEWE));
	EEAR = dir;
	EECR |= (1<<EERE);
	return EEDR;
}

void pool_data() {
	int8_t r;
	LCD_CMD_8BIT(LCD_Cmd_Clear); 
	for(int i = 0; i < 4; i++){
		r = EEPROM_read(data_addr[i]);
		LCD_WR_CHAR(r + '0');
	}
}

int main(void)
{
    LCD_inicialization();
    config_kbd();
	pool_data(); 
    while (1) 
    {
		int8_t r = read_kbd(); 
		if(r != -1) {
			for(int i = 3; i >= 1; i--) 
				EEPROM_write(data_addr[i], EEPROM_read(data_addr[i - 1])); 
			EEPROM_write(data_addr[0], r); 
			pool_data(); 
		}
    }
}

