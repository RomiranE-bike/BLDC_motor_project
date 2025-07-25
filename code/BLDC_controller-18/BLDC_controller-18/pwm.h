/*
 * pwm.h
 *
 * Created: 6/1/2024 16:21:39
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
#ifndef PWM_H_
#define PWM_H_
//--------------------------------------------------------------------------------------------------------------
#define PWM_max_value      255
#define PWM_min_value      35
#define PWM_value          35
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
//--------------------------------------------------------------------------------------------------------------
//set fast PWM mode
void pwm_init(){
	
	//DDRB  |= (1<<PB3);  //set PB3 OC0 pin as PWM output
	TCCR0 |= (1<<WGM01) | (1<<WGM00); //set fast PWM mode with non-inverted output
	TCCR0 |= (1<<COM01);//Clear OC0 on compare match when up-counting. Set OC0 on compare match when down counting.
	TCCR0 |= (1<<CS00);//No prescaling
	
	// T1 and T2 PWM
	//DDRD |= (1 << PD4) | (1 << PD5); //Pin OC1A & B set to output
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);//Clear OC1A/OC1B on compare match, set OC1A/OC1B at BOTTOM, (non-inverting mode)
	TCCR1A |= (1 << WGM10);//Fast PWM mode, 8-bit
	TCCR1B |= (1 << WGM12);//Fast PWM mode, 8-bit
	TCCR1B |= (1<<CS10);//No prescaling
	
	//DDRD  |= (1<<PD7);//set PD7 OC2 as PWM output
	TCCR2 |= (1<<WGM21)|(1<<WGM20);//select Fast PWM mode
	TCCR2 |= (1<<COM21);//clear OC2 on compare match
	TCCR2 |= (1<<CS20);//No prescaling	
	
}
//--------------------------------------------------------------------------------------------------------------
void SET_PWM(int speed){

		OCR1A  = speed;                   // Set OC1A  PWM duty cycle
		OCR1B  = speed;                   // Set OC1B  PWM duty cycle
		OCR2   = speed;                   // Set OC2   PWM duty cycle
}
//--------------------------------------------------------------------------------------------------------------
#endif // PWM_H_ 