/*
 * interrupt.h
 *
 * Created: 6/1/2024 16:17:45
 *  editor: me
 */ 
#ifndef INTERRUPT_H_
#define INTERRUPT_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include "beep.h"
#include "io.h"
//#include "commutation.h"
//#include "setting.h"
//#include "pwm.h"
//--------------------------------------------------------------------------------------------------------------
unsigned long        counter_1, current_count;//We create variables for the time width value of the PWM input signal
int                  last_PWM_state;//To store the 1000us to 2000us value we create variables
int                  PWM_INPUT;      //In my case PWM_IN pin is D8 of the Arduino
unsigned int         i;
//int                  sequence_step = 0;
//--------------------------------------------------------------------------------------------------------------
void timer0_init();
void Comparator_init();
//--------------------------------------------------------------------------------------------------------------
void timer0_init(){
	
	
	TCNT0 = 0;// load TCNT0
	//TCCR0 |= (1 << CS02)|(1 << CS00); // Prescaler by 1024
	TCCR0   |= (1 << CS00); // No prescaling
	TIMSK |= (1 << TOIE0); // Enable T0 overflow interrupt
	
}
//--------------------------------------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect){
	
	motor_spining();
}
//--------------------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//the Analog comparator

void Comparator_init(void){
		
	ACSR &= 0x00;		// Clear ACSR register 
	// Analog comparator
	ADCSRA &= ~(1 << ADEN);// Turn off the ADC
	SFIOR  |=  (1 << ACME); // Connect the negative input of the comparator to the output of the ADC multiplexer
	ACSR   |=  (1 << ACIE); // Enable interrupts from the comparator	
}
//--------------------------------------------------------------------------------------------------------------
// Comparator interrupt handler. Back EMF detector
/*
ISR(ANA_COMP_vect){
	
	rotor_run ++; // we increment the impulses
	_delay_us(1);
	if(rotor_run > 200){ 
	  rotor_run = 200;
	  Write_String("200");
	   }
    if(rotor_run == 200){ // If back EMF pulses are present, turn full  
	commutation(0); // We switch the windings according to the comparator signal
	PORTC ^=(1<<PC1);	// Then	turn on/off PC1 pin	
	Write_String(" BLDC IS RUNNING !");
	}
		
}
*/
//--------------------------------------------------------------------------------------------------------------
// The ISR vector of the Analog comparator
/*
ISR (ANA_COMP_vect) {

	for(i = 0; i < 10; i++) {                 //We check the comparator output 10 times to be sure
		if(sequence_step & 1){                //If step = odd (0001, 0011, 0101) 1, 3 or 5
			if(!(ACSR & (1<<ACO))) i -= 1;   //!0b00100000 -> 0b11011111 ACO = 0 (Analog Comparator Output = 0)
		}
		else{                              //else if step is 0, 2 or 4
			if((ACSR & (1<<ACO)))  i -= 1; //else if 0b00100000 -> 0b11011111 ACO = 1 (Analog Comparator Output = 1)
		}
	}
	
	BLDC_turning();                     //set the next step of the sequence
	sequence_step ++;                   //increment step by 1, next part of the sequence of 6
	sequence_step %= 6;                 //If step > 5 (equal to 6) then step = 0 and start over	
}
*/
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
// T0 overflow interrupt handler. Motor operation without back EMF signals
// If the interruption is triggered, there are missing back EMF pulses
/*
ISR(TIMER0_OVF_vect){
	
	//rotor_run = 0; // Resetting the pulse counter	
	//OCR1A = START_PWM; // PWM minimum
	//OCR1B = START_PWM;
	//OCR2 = START_PWM;
	commutation(1); //We switch the windings unconditionally
	PORTC ^=(1<<PC0);	// Turn ON/off PC0 pin
	Write_String("  soft_start>>>");
}
*/
//--------------------------------------------------------------------------------------------------------------
//T1 overflow interrupt handler
/*
ISR(TIMER1_OVF_vect){
	//First we take the current count value in micro seconds using the micros() function
	current_count = 150;//micros();
	///////////////////////////////////////Channel 1
	if(PINB & 0b00000001){                              //We make an AND with the pin state register, We verify if pin 8 is HIGH???
		if(last_PWM_state == 0){                         //If the last state was 0, then we have a state change...
			last_PWM_state = 1;                            //Store the current state into the last state for the next loop
			counter_1 = current_count;                     //Set counter_1 to current value.
		}
	}
	else if(last_PWM_state == 1){                      //If pin 8 is LOW and the last state was HIGH then we have a state change
		last_PWM_state = 0;                              //Store the current state into the last state for the next loop
		PWM_INPUT = current_count - counter_1;           //We make the time difference. Channel 1 is current_time - timer_1.
	}
}
*/
//--------------------------------------------------------------------------------------------------------------
#endif // INTERRUPT_H_ 
//--------------------------------------------------------------------------------------------------------------