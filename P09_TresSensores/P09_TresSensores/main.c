/*
 * P09_TresSensores.c
 *
 * Created: 08/03/2022 03:39:29 p. m.
 * Author : ricar
 */ 

#define F_CPU 4000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

void saca_uno(volatile uint8_t *LUGAR, uint8_t BIT){
	*LUGAR=*LUGAR|(1<<BIT);
}

uint8_t uno_en_bit(volatile uint8_t *LUGAR, uint8_t BIT){
	return ((*LUGAR&(1<<BIT)));
}

uint16_t lectura_adc(uint8_t adcbit) {
	ADMUX &= 0b11100000;
	ADMUX |= adcbit;
	saca_uno(&ADCSRA, 6);
	while(uno_en_bit(&ADCSRA, 6));
	return ADC; 
}

uint8_t map_led(uint16_t val) {
	PORTD = val; 
	if(val < 205) return 0;
	if(val < 410) return 1;
	if(val < 614) return 3;
	if(val < 819) return 7;
	return 15; 
}

int main(void)
{
    // Entradas y salidas
	DDRB = DDRC = 255;
	DDRD = 255;
	
	// ADC
	ADMUX = 0b01000000;
	SFIOR = 0;
	ADCSRA = 0b10010101; 
	uint16_t r = 0;
    while (1) 
    {
		r = lectura_adc(0); // Temperatura
		PORTB &= 0b11110000;
		PORTB |= map_led(r); 
		_delay_ms(50); 
		
		r = lectura_adc(1); // Humedad
		PORTB &= 0b00001111;
		PORTB |= (map_led(r) << 4);
		_delay_ms(50);
		
		r = lectura_adc(2); // pH
		PORTC = map_led(r); 
		_delay_ms(50);
    }
}

