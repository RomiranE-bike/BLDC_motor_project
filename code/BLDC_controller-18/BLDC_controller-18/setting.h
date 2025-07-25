/*
 * setting.h
 *
 * Created: 6/1/2024 15:30:31
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
#ifndef SETTING_H_
#define SETTING_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
#include "beep.h"
//--------------------------------------------------------------------------------------------------------------
unsigned char speed;
//--------------------------------------------------------------------------------------------------------------
void setting(){
	
	// T0 - for starting and operating the engine without back EMF signals
	//TCCR0   |= (1 << CS02)|(1 << CS00); // Prescaler by 1024
	//TIMSK   |= (1 << TOIE0); // Enable T0 overflow interrupt
	
	// T1 and T2 PWM
	TCCR1A  |= (1 << COM1A1)|(1 << COM1B1); // Clear OC1A/OC1B, set OC1A/OC1B at BOTTOM
	TCCR1A  |= (1 << WGM10); // Fast PWM mode, 8-bit
	TCCR1B  |= (1 << CS10)|(1 << WGM12); // Without prescaler
	TCCR2   |= (1 << COM21); // Clear OC2, set OC2 at BOTTOM
	TCCR2   |= (1 << WGM21)|(1 << WGM20); // Fast PWM mode
	TCCR2   |= (1 << CS20); // Without prescaler	
	
	// Analog comparator
	//ADCSRA &= ~(1 << ADEN); // Turn off the ADC
	//SFIOR  |= (1 << ACME); // Connect the negative input of the comparator to the output of the ADC multiplexer
	//ACSR   |= (1 << ACIE); // Enable interrupts from the comparator
	
	// External interrupt (Encoder)
	//MCUCR |= (1 << ISC01); // Interrupt on the falling edge of INT0 (on the falling edge of the pulse)
	//GIFR  |= (1 << INTF0); // Clear the external interrupt flag
	//GICR  |= (1 << INT0); // Enable external interrupts INT0
	
	sei(); // Globally enable interrupts

//--------------------------------------------------------------------------------------------------------------
// Setting up the ADC
void ADC_init(){
	ADMUX |= (1 << REFS0); // Reference voltage AVcc with capacitor on (AREF) pin
	ADMUX |=(1 << ADLAR); // Align the result to the left, read only ADCH
	ADMUX |=(1 << MUX1)|(1 << MUX0); // ADC3 input
	ADCSRA |= (1 << ADEN) | (1 << ADPS1); // Enable ADC, prescaler by 4
}
//--------------------------------------------------------------------------------------------------------------
//ADC conversion
void ADC_read(){
	ADCSRA |= (1 << ADSC); // Start ADC conversion
	while ( ADCSRA & (1 << ADSC) ); // Wait for the conversion to complete
	speed = ADCH; // Read the ADC
}
//--------------------------------------------------------------------------------------------------------------
}//End of setup loop
//--------------------------------------------------------------------------------------------------------------
#endif // SETTING_H_
//--------------------------------------------------------------------------------------------------------------