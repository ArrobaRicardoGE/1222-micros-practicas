/*
 * P10_ADCTrigger.c
 *
 * Created: 10/03/2022 03:22:29 p. m.
 * Author : ricar
 */ 

#define F_CPU 1000000
#define ADC_vect _VECTOR(14)
#define TIMER0_COMP_vect _VECTOR(19)
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int x = 0, y = 0; 

ISR (ADC_vect) {
	OCR2 = (int)((double)ADC * 255.0 / 1024.0);
}

ISR (TIMER0_COMP_vect) {}
	
int main(void)
{	
	// Habilitar interrupcion
	sei();
		
    // Configurar ADC
	ADMUX = 0b01000010;
	SFIOR = 0b01100000; // Tim0 comp
	ADCSRA = 0b10111011; // Trigger, INT, 8
	
	// Configurar timer 0
	OCR0 = 97; 
	TCCR0 = 0b00001101; // prescaler 1024
	TIMSK = 0b00000010;
	
	// Configurar PWM en OCR2 prescaler 64
	TCCR2 |= (1<<WGM20)|(1<<WGM21);                   
	TCCR2 |= (1<<COM21)|(1<<CS20)|(1<<CS21)|(0<<CS22);
	
	OCR2 = 0;
	
	// Puertos
	DDRD = 255; 
	DDRC = 255; 
	 
    while (1) {}
}

