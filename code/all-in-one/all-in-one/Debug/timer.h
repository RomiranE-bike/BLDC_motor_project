/*
 * timer.h
 *
 * Created: 8/30/2024 14:41:48
 *  Author: User
 */ 
//----------------------------------------------------------------------------------------------------------------
#ifndef TIMER_H_
#define TIMER_H_
//----------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
//----------------------------------------------------------------------------------------------------------------
//Timer0 initialization
void timer0_init(void);
//Timer1 initialization
void timer1_init(void);
//Timer1 initialization
void timer2_init(void);
//----------------------------------------------------------------------------------------------------------------
void timer0_init(void){
	
	// T0 - for starting and operating the menu items
	/*
	CS02 CS01 CS00 Description
	0 0 0 No clock source (Timer/Counter stopped).
	0 0 1 clkI/O/(No prescaling)
	0 1 0 clkI/O/8 (From prescaler)
	0 1 1 clkI/O/64 (From prescaler)
	1 0 0 clkI/O/256 (From prescaler)
	1 0 1 clkI/O/1024 (From prescaler)
	1 1 0 External clock source on T0 pin. Clock on falling edge.
	1 1 1 External clock source on T0 pin. Clock on rising edge.
	*/
	
	TCNT0 = 0;// load TCNT0
	TCCR0 =0x05; //Prescaler by 1024  (if 256 it means  ~122 interrupts/s)
	TIMSK   |= (1 << TOIE0); // Enable T0 overflow interrupt
}
//----------------------------------------------------------------------------------------------------------------
void timer1_init(void){
	
	// T1 - for starting and operating the engine without back EMF signals
	/*
	CS12 CS11 CS10 Description
	0 0 0 No clock source (Timer/Counter stopped).
	0 0 1 clkI/O/1 (No prescaling)
	0 1 0 clkI/O/8 (From prescaler)
	0 1 1 clkI/O/64 (From prescaler)
	1 0 0 clkI/O/256 (From prescaler)
	1 0 1 clkI/O/1024 (From prescaler)
	1 1 0 External clock source on T1 pin. Clock on falling edge.
	1 1 1 External clock source on T1 pin. Clock on rising edge.
	*/
	TCNT1  = 0;// load TCNT1
	TCCR1A = 0;
	TCCR1B = 0x01;// Prescaler by 1
	TIMSK |= (1 << TOIE1); // Enable T0 overflow interrupt
}
//----------------------------------------------------------------------------------------------------------------
void timer2_init(void){	
	// T2 - for starting and operating the engine with back EMF signals
	/*
	CS22 CS21 CS20 Description
	0 0 0 No clock source (Timer/Counter stopped).
	0 0 1 clkT2S/(No prescaling)
	0 1 0 clkT2S/8 (From prescaler)
	0 1 1 clkT2S/32 (From prescaler)
	1 0 0 clkT2S/64 (From prescaler)
	1 0 1 clkT2S/128 (From prescaler)
	1 1 0 clkT2S/256 (From prescaler)
	1 1 1 clkT2S/1024 (From prescaler)
	*/
	TCNT2 = 0;// load TCNT0
	TCCR2 = 0x6;//Prescaler by 256 
	TIMSK |= (1 << TOIE2); // Enable T2 overflow interrupt
}
//----------------------------------------------------------------------------------------------------------------
#endif /* TIMER_H_ */
//----------------------------------------------------------------------------------------------------------------