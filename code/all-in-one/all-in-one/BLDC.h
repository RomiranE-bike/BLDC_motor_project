/*
 * BLDC.h
 *
 * Created: 8/30/2024 14:41:48
 *  Author: me
 */ 

//--------------------------------------------------------------------------------------------------------------
#ifndef BLDC_H_
#define BLDC_H_
//--------------------------------------------------------------------------------------------------------------
// Connecting a BLDC motor to an AVR atmega16 (without sensors) 
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
//----------------------------------------------------------------------------------------------------------------
#include "io.h"
#include "LCD.h"
#include "beep.h"
#include "NTC.h"
#include "motor_init.h"
#include "timer.h"
//----------------------------------------------------------------------------------------------------------------
void soft_start(void);
void motor_run(void);
void BLDC_off(void);
void BLDC_stop(void);
//----------------------------------------------------------------------------------------------------------------
// Rotate clockwise
void commutation(char startup){
	
	switch (sequence_step){
		
		case (0):
		if(!(ACSR&(1 << ACO)) || startup){
			step1;
			ADMUX = 2; // Activate the W phase input
			sequence_step = 1; // Next step
			TCNT2 = 0; // Reset counter T2
		    }
		break;	
		case (1):
		if((ACSR&(1 << ACO)) || startup){
			step2;
			ADMUX = 1; // Activate phase V input
			sequence_step = 2;
			TCNT2 = 0; // Reset counter T2
		   }
		break;
		
		case (2):
		if(!(ACSR &(1 << ACO)) || startup){
			step3;
			ADMUX = 0; // Activate phase U input
			sequence_step = 3;
			TCNT2 = 0; // Reset counter T2
		  }
		break;
		
		case (3):
		if((ACSR &(1 << ACO)) || startup){
			step4;
			ADMUX = 2; // Activate the W phase input
			sequence_step = 4;
			TCNT1 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		   }
		break;
		
		case (4):
		if(!(ACSR &(1 << ACO)) || startup){
			step5;
			ADMUX = 1; // Activate phase V input
			sequence_step = 5;
			TCNT2 = 0; // Reset counter T2
		   }
		break;
		
		case(5):
		if((ACSR &(1 << ACO)) || startup){
			step6;
			ADMUX = 0; // Activate phase U input
			sequence_step = 0;
			TCNT2 = 0; // Reset counter T2
		   }
		break;
	}
}
//----------------------------------------------------------------------------------------------------------------
                  //////////////////////////// soft_start ///////////////////////
void soft_start(void){
		
				int i = 2200;
				// Motor start
				while( (i > 100) && ((B_inpout_PIN &(1<<Enter_start))==0) ) {
					for(int j=0;j<i;j++){
						_delay_us(200);
					}//end of for
					commutation(1);
					sequence_step++;
					sequence_step %= 6;
					i = i - 20;
				  }//end of while
			   //ACSR   |= (1 << ACIE); // Enable analog comparator interrupt				  
		       if((B_inpout_PIN &(1<<Enter_start))) PHASE_ALL_OFF;//motor off					  			
}
//----------------------------------------------------------------------------------------------------------------						
				//////////////////////////// motor_run///////////////////////
void motor_run(void){	
	
			timer2_init();//initial timer2 for  overflow interrupts	        
			
	        PHASE_ALL_OFF; // Turn off all phases
			
			ADCSRA &= ~(1 << ADEN);// Turn off the ADC//Comparator initial for back EMF
	        SFIOR |= (1 << ACME); // Connect the negative input of the comparator to the output of the ADC multiplexer
			ACSR   |= (1 << ACIE);  // Enable analog comparator interrupt
			
			sei(); //  enable interrupts									
			
	        while(Flag  && (B_inpout_PIN &(1<<Exit_stop)) && (thermo_sens < thermo_limit) ){	
					
					position = ADMUX;	// save position to the saved buffer
					ACSR &= 0x00;		// Clear ACSR register
					ADCSRA &= ~(1 << ADEN);// Turn off the ADC
					SFIOR  |= (1 << ACME); // Connect the negative input of the comparator to the output of the ADC multiplexer
					ADMUX = position;// set position with the saved buffer
					sei(); //  enable interrupts
					ACSR   |= (1 << ACIE);  // Enable interrupt from comparator
					TIMSK  |= (1 << TOIE2); // Enable T0 overflow interrupt	
						    
				    if((B_inpout_PIN &(1<<Exit_stop))==0)break;

				}//end of while
				PHASE_ALL_OFF;//motor off
}
//----------------------------------------------------------------------------------------------------------------
//back EMF zero crossing detection
ISR (ANA_COMP_vect){	
	rotor_run++;
	if(rotor_run > 200)rotor_run = 200;
	if(rotor_run == 200) commutation(0);
	spinning_flag=1;
}
//----------------------------------------------------------------------------------------------------------------
// T2 overflow interrupt handler. Motor operation with back EMF signals
ISR(TIMER2_OVF_vect){	
	commutation(1); // If the interrupt is triggered, there are missing back-EMF pulses
	rotor_run = 0;   // Reset the pulse counter	
	spinning_flag=0;	
}
//----------------------------------------------------------------------------------------------------------------
                //////////////////////////// Motor off///////////////////////
void BLDC_off(void){
	PHASE_ALL_OFF;//motor off
	ACSR  &= ~(1 << ACIE);  // Disable the comparator interrupt
	TCNT1=0;
	TCNT2=0;
	TCCR1A=0;
	TCCR1B=0;
	TCCR2=0;
	TIMSK &= ~(1 << TOIE1); // Disable T0 overflow interrupt
	TIMSK &= ~(1 << TOIE2); // Disable T2 overflow interrupt
	blink();
}
//----------------------------------------------------------------------------------------------------------------
                //////////////////////////// Motor stop///////////////////////
void BLDC_stop(void){
	                
	while( Flag  && (B_inpout_PIN &(1<<Exit_stop)) && (thermo_sens < thermo_limit) ){
		BLDC_off();
	}
}

//----------------------------------------------------------------------------------------------------------------
#endif /* BLDC_H_ */
//----------------------------------------------------------------------------------------------------------------