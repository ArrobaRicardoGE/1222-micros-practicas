/*
 * Final.c
 *
 * Created: 08/05/2022 04:25:50 p. m.
 * Author : Ricardo Gutierrez
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "util.h"
#include "lcd.h"
#include "i2c.h"

#define PCS_WRITE_ADDR (0x29 << 1)
#define PCS_READ_ADDR ((0x29 << 1) + 1)

uint8_t volatile color_table[256]; 


void show_number(uint8_t x) {
	int pad = 3; 
	char ans[pad + 1]; 
	ans[pad--] = 0;
	while(x > 0) {
		ans[pad--] = (x % 10) + '0';
		x /= 10;
	}
	while(pad >= 0) {
		ans[pad--] = '0'; 
	}
	LCD_WR_string(ans); 
}

void show_rgb(uint8_t r, uint8_t g, uint8_t b) {
	for(int i = 0; i < 14; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftL); 
	show_number(r);  
	for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
	show_number(g);
	for(int i = 0; i < 2; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
	show_number(b);
	for(int i = 0; i < 1; i++) LCD_CMD_8BIT(LCD_Cmd_ShiftR); 
}

void write_cmd_8(uint8_t cmd, uint8_t val) {
	i2c_start_condition();
	i2c_set_addr(PCS_WRITE_ADDR);
	i2c_write_data((0x80 | cmd));	// mask with command byte
	i2c_write_data(val);
	i2c_stop_condition();
}

uint8_t read_cmd_8(uint8_t cmd) {
	i2c_start_condition(); 
	i2c_set_addr(PCS_WRITE_ADDR); 
	i2c_write_data((0x80 | cmd));	// mask with command byte
	i2c_start_condition();			// repeated start
	i2c_set_addr(PCS_READ_ADDR); 
	uint8_t ans = i2c_read_nack();
	i2c_stop_condition(); 
	return ans; 
}

uint16_t read_color_16(uint8_t color) {
	i2c_start_condition();
	i2c_set_addr(PCS_WRITE_ADDR);
	i2c_write_data((0xA0 | color));
	i2c_start_condition();
	i2c_set_addr(PCS_READ_ADDR);
	uint16_t low = i2c_read_ack();
	uint16_t high = (uint16_t)i2c_read_nack(); 
	high <<= 8; 
	i2c_stop_condition();
	return high | low;
}

void enable() {
	write_cmd_8(0x00, 0x01);			// PON
	_delay_ms(3); 
	write_cmd_8(0x00, 0x01 | 0x02);		// PON | AEN
	_delay_ms(700); 
}

void get_raw_values(uint16_t *r, uint16_t *g, uint16_t *b, uint16_t *c) {
	*c = read_color_16(0x14); 
	*r = read_color_16(0x16); 
	*g = read_color_16(0x18); 
	*b = read_color_16(0x1A); 
	_delay_ms(700); 
}

void get_rgb(uint8_t *r, uint8_t *g, uint8_t *b) {
	uint16_t _r, _g, _b, _c; 
	get_raw_values(&_r, &_g, &_b, &_c); 
	
	// If no light
	if(_c == 0) {
		*r = *g = *b = 0; 
		return;
	}
	
	*r = (float)_r / (float)_c * 255.0; 
	*g = (float)_g / (float)_c * 255.0; 
	*b = (float)_b / (float)_c * 255.0; 
}

int main(void)
{
	// LCD
	LCD_inicialization();
	
	// PWM
	TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(0<<CS02)|(0<<CS01)|(1<<CS00);
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
	ICR1=255;
	
	DDRB|=(1<<3);
	DDRD|=(1<<5)|(1<<4);
	
	OCR0=255;
	OCR1A=255;
	OCR1B=255;
	
	// I2C - TCS34725
	i2c_init(); 
	while(read_cmd_8(0x12) != 0x44) {
		LCD_CMD_8BIT(LCD_Cmd_Clear); 
		LCD_WR_string("TCS not found"); 
		_delay_ms(1000); 
	}
	// Set integration time and gain
	write_cmd_8(0x01, 0x00); // 700ms 
	write_cmd_8(0x0F, 0x01); // 4x
	
	enable(); 
	uint8_t r, g, b;
	LCD_CMD_8BIT(LCD_Cmd_Clear); 
	LCD_WR_string("(000, 000, 000)"); 
	
	// Fill color table
	for (int i=0; i<256; i++)
	{
		float x = i;
		x /= 255;
		x = pow(x, 2.5);
		x *= 255;
		color_table[i] = x;
	}
	
	while (1)
	{
		get_rgb(&r, &g, &b); 
		show_rgb(r, g, b); 
		OCR0 = color_table[r];
		OCR1A = color_table[g];
		OCR1B = color_table[b];
	}
}

