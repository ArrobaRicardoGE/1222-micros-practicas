#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

#include <avr/io.h>
#include <avr/interrupt.h>
#include "util.h"

void EEPROM_write(uint16_t dir, uint8_t dato, int interrupt);
uint8_t EEPROM_read(uint16_t dir); 


#endif