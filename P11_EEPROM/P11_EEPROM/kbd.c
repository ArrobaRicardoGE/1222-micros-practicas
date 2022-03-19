#include "kbd.h"

// Cada arreglo es una columna, cada valor va del bit mas bajo al mas alto
volatile uint8_t cols[] = {0,1,2,3};
volatile uint8_t rows[] = {4,5,6,7};
	
volatile int8_t values[KBD_NCOLS][KBD_NROWS] = {	{7, 8, 9, -1},	{4, 5, 6, -1},	{1, 2, 3, -1},	{-1, 0, -1, -1},};

void config_kbd() {
	DDRKBD = 0b00001111;
	PORTKBD = 255;
}

int8_t read_kbd() {
	int8_t reading = -1;
	
	while(reading == -1) {
		for(uint8_t i = 0; i < KBD_NCOLS; i++) {
			PORTKBD = 0b11111111 ^ (1 << cols[i]);
			_delay_ms(50);
			for(uint8_t j = 0; j < KBD_NROWS; j++){
				if(cero_en_bit(&PINKBD, rows[j])) {
					reading = values[i][j];
					while(cero_en_bit(&PINKBD, rows[j]));
				}
			}
		}
	}
	
	return reading;
}