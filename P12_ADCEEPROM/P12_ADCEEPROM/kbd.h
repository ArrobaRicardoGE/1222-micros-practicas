#ifndef KBD_H_INCLUDED
#define KBD_H_INCLUDED

#define DDRKBD DDRD
#define PORTKBD PORTD
#define PINKBD PIND
#define KBD_NCOLS 4 
#define KBD_NROWS 4

#define F_CPU 1000000
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include "util.h"

void config_kbd();
int8_t read_kbd();


#endif