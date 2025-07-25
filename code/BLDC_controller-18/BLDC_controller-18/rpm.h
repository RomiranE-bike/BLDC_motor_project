/*
 * rpm.h
 *
 * Created: 7/7/2024 11:09:25
 *  Author: User
 */ 
//--------------------------------------------------------------------------------------------------------------

#ifndef RPM_H_
#define RPM_H_
//--------------------------------------------------------------------------------------------------------------
// Measures RPM, pulses on INT0 (PD2)
// One pulse for revolution
// Range 30 to >10000 rpm
//Atmega8
#define F_CPU  8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "io.h"
#include "LCD.h"
#include "beep.h"
//--------------------------------------------------------------------------------------------------------------
//One timer step lasts
//timer_step[micro sec] = prescaler / F_CPU[MHz]
#define TIMER_STEP  256/8
volatile uint8_t    overflow;
volatile uint32_t   cycle;
uint16_t            rpm;
//--------------------------------------------------------------------------------------------------------------
uint16_t tacho_init(void);
//--------------------------------------------------------------------------------------------------------------
uint16_t tacho_init(void){
	
	MCUCR |= (1<<ISC01)|(1<<ISC00);//rising edge
	GICR  |= (1<<INT0);  //enable INT0 interrupt
	TCCR0 |= (1<<CS02);  //prescaler 256
	TIMSK |= (1<<TOIE0); //enable tmr0 OVF interrupt
	
		cli(); //atomic section
		rpm = F_CPU * 60 / cycle;
		sei(); //atomic section end
		_delay_ms(20);
		
		Set_Cursor(1,1);
		Write_String(" RPM= ");
		print_value(rpm);

	return rpm;
}
//--------------------------------------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect){
	overflow++;
}
//--------------------------------------------------------------------------------------------------------------
ISR(INT0_vect){
	
	static uint8_t pulse;
	if(pulse == 1){
		overflow = 0;      //clear overflow counter
		TCNT0 = 0;         //clear timer
		//TCCR0 = (1<<CS02); //start timer0
		TCCR0 = (1<<CS01); //start timer0
		pulse = 0;
	    }
	else{
		TCCR0 = 0;         //stop timer0
		cycle = (overflow * 256 + TCNT0) * TIMER_STEP ; //[#define TIMER_STEP  256/8 micro sec]
		pulse = 1;
	}
}
//--------------------------------------------------------------------------------------------------------------
#endif /* RPM_H_ */
//--------------------------------------------------------------------------------------------------------------