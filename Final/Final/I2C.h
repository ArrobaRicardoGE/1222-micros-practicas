#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdint.h>
#include "lcd.h"	//debugging
#define SCL_CLK 100000
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

void i2c_init(); 
void i2c_start_condition(); 
void i2c_set_addr(uint8_t addr);
void i2c_write_data(uint8_t data); 
void i2c_stop_condition(); 
uint8_t i2c_read_nack(); 
uint8_t i2c_read_ack(); 

#endif