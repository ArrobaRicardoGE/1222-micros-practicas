#include "EEPROM.h"

void EEPROM_write(uint16_t dir, uint8_t dato, int interrupt){
	while (uno_en_bit(&EECR, EEWE));
	EEAR = dir;
	EEDR = dato;
	if(interrupt) cli();
	EECR |= (1<<EEMWE);
	EECR |= (1<<EEWE);
	if(interrupt) sei();
}

uint8_t EEPROM_read(uint16_t dir){
	while (uno_en_bit(&EECR, EEWE));
	EEAR = dir;
	EECR |= (1<<EERE);
	return EEDR;
}