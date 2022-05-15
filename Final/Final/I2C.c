#include "i2c.h"


void i2c_init() {
	PORTC = 255;
	TWBR = BITRATE(TWSR = 0x00);
}

void i2c_start_condition() {
	TWCR = (1 << TWINT)|(1 << TWSTA)|(1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	while((TWSR & 0xF8) != 0x08 && (TWSR & 0xF8) != 0x10) {
		LCD_CMD_8BIT(LCD_Cmd_Clear);
		LCD_WR_string("Failed to start");
		_delay_ms(1000);
	}
}

void i2c_set_addr(uint8_t addr) {
	TWDR = addr;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	// check status
}

void i2c_write_data(uint8_t data) {
	TWDR = data;
	TWCR = (1 << TWINT)|(1 << TWEN);
	while(!(TWCR & (1 << TWINT)));
	// check status
}

void i2c_stop_condition() {
	TWCR = (1 << TWINT)|(1 << TWEN)|(1 << TWSTO);
}

uint8_t i2c_read_nack() {
	TWCR = (1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

uint8_t i2c_read_ack() {
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}