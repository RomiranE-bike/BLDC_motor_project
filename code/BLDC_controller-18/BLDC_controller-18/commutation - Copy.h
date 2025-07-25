/*
 * commutation.h
 *
 * Created: 6/1/2024 14:22:56
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
#ifndef COMMUTATION_H_
#define COMMUTATION_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include "io.h"
#include "pwm.h"
//--------------------------------------------------------------------------------------------------------------
unsigned int  sequence_step ;
//--------------------------------------------------------------------------------------------------------------
void BLDC_turning(){
	switch(sequence_step){
		case 0:
		AH_BL();
		BEMF_C_RISING();
		//_delay_ms(10);
		//sequence_step = 1; // Next step
		//TCNT0 = 0; // Reset counter T1
		break;
		case 1:
		AH_CL();
		BEMF_B_FALLING();
		//_delay_ms(10);
		//sequence_step = 2; // Next step
		//TCNT0 = 0; // Reset counter T1
		break;
		case 2:
		BH_CL();
		BEMF_A_RISING();
		//_delay_ms(10);
		//sequence_step = 3; // Next step
		//TCNT0 = 0; // Reset counter T1
		break;
		case 3:
		BH_AL();
		BEMF_C_FALLING();
		//_delay_ms(10);
		//sequence_step = 4; // Next step
		//TCNT0 = 0; // Reset counter T1
		break;
		case 4:
		CH_AL();
		BEMF_B_RISING();
		//_delay_ms(10);
		//sequence_step = 5; // Next step
		//TCNT0 = 0; // Reset counter T1
		break;
		case 5:
		CH_BL();
		BEMF_A_FALLING();
		//_delay_ms(10);
		//sequence_step = 0; // Next step
		//TCNT0 = 0; // Reset counter T1
		break;
	}
}//end of BLDC_move
//--------------------------------------------------------------------------------------------------------------
//phase U BEMF pulse rising
void BEMF_A_RISING(){
	ADCSRA = (0 << ADEN);     // Disable the ADC module
	SFIOR =  (1 << ACME);     // Enable MUX select for negative input of comparator
	ADMUX = 2;                // Select ADC2 as comparator negative input
	ACSR |= (1<<ACIS1)| (1<<ACIS0);       //Comparator Interrupt on Rising Output Edge
}
//--------------------------------------------------------------------------------------------------------------
//phase U BEMF pulse falling
void BEMF_A_FALLING(){
	ADCSRA = (0 << ADEN);     // Disable the ADC module
	SFIOR = (1 << ACME);     // Enable MUX select for negative input of comparator
	ADMUX = 2;                // Select ADC2 as comparator negative input
	ACSR |= (1<<ACIS1)| (0<<ACIS0);            // Comparator Interrupt on Falling Output Edge
}
//--------------------------------------------------------------------------------------------------------------	
//phase V BEMF pulse rising
void BEMF_B_RISING(){
	ADCSRA = (0 << ADEN);     // Disable the ADC module
	SFIOR = (1 << ACME);     // Enable MUX select for negative input of comparator
	ADMUX = 3;                // Select ADC3 as comparator negative input
	ACSR |= (1<<ACIS1)| (1<<ACIS0);       //Comparator Interrupt on Rising Output Edge
}
//--------------------------------------------------------------------------------------------------------------
//phase V BEMF pulse falling	
void BEMF_B_FALLING(){
	ADCSRA = (0 << ADEN);     // Disable the ADC module
	SFIOR = (1 << ACME);     // Enable MUX select for negative input of comparator
	ADMUX = 3;                // Select ADC3 as comparator negative input
	ACSR |= (1<<ACIS1)| (0<<ACIS0);            // Comparator Interrupt on Falling Output Edge
}
//--------------------------------------------------------------------------------------------------------------
//phase W BEMF pulse rising	
void BEMF_C_RISING(){
	ADCSRA = (0 << ADEN);     // Disable the ADC module
	SFIOR = (1 << ACME);     // Enable MUX select for negative input of comparator
	ADMUX = 4;                // Select ADC4 as comparator negative input
	ACSR |= (1<<ACIS1)| (1<<ACIS0);       //Comparator Interrupt on Rising Output Edge
}
//--------------------------------------------------------------------------------------------------------------
//phase W BEMF pulse falling	
void BEMF_C_FALLING(){
	ADCSRA = (0 << ADEN);     // Disable the ADC module
	SFIOR = (1 << ACME);     // Enable MUX select for negative input of comparator
	ADMUX = 4;                // Select ADC4 as comparator negative input
	ACSR |= (1<<ACIS1)| (0<<ACIS0);            // Comparator Interrupt on Falling Output Edge
}
//--------------------------------------------------------------------------------------------------------------	
//PD5 PWM and PC3 HIGH.
void AH_BL(){
PORTC   = (0 << PC4)|(1 << PC3)|(0<< PC2);     //Set PC3 to HIGH and the rest to LOW
TCCR2    =  0;               //OC2 - PD7 normal port.
//TCCR1A   =  0x81;           //OC1A - PD5 compare match non inverting mode, down counting ,PWM 8-bit
TCCR1A |= (1 << COM1A1);//Clear OC1A on compare match, set OC1A at BOTTOM, (non-inverting mode)
TCCR1A |= (1 << WGM10);//PWM, Phase Correct, 8-bit
}
//--------------------------------------------------------------------------------------------------------------
//PD5 PWM and PC4 HIGH
void AH_CL(){
PORTC   = (1 << PC4)|(0 << PC3)|(0 << PC2);     //Set PC4 to HIGH and the rest to LOW
TCCR2    =  0;              //OC2 - PD7 normal port.
//TCCR1A   =  0x81;           //OC1A - PD5 compare match non inverting mode, down counting ,PWM 8-bit
TCCR1A |= (1 << COM1A1);//Clear OC1A on compare match, set OC1A at BOTTOM, (non-inverting mode)
TCCR1A |= (1 << WGM10);//PWM, Phase Correct, 8-bit
}
//--------------------------------------------------------------------------------------------------------------
//PD4 PWM and PC4 HIGH
void BH_CL(){
PORTC  = (1 << PC4)|(0 << PC3)|(0 << PC2);     //Set PC4 to HIGH and the rest to LOW
TCCR2   =  0;              //OC2 - PD7 normal port.
//TCCR1A  =  0x21;          //0b00100001 OC1B - PD4 compare match non inverting mode, down counting ,PWM 8-bit
TCCR1A |=  (1 << COM1B1);//Clear OC1B on compare match, set OC1B at BOTTOM, (non-inverting mode)
TCCR1A |=  (1 << WGM10);//PWM, Phase Correct, 8-bit
}
//--------------------------------------------------------------------------------------------------------------
//PD4 PWM and PC2 HIGH
void BH_AL(){
PORTC  = (0 << PC4)+(0 << PC3)+(1 << PC2);      //Set PC2 to HIGH and the rest to LOW
TCCR2   =  0;              //OC2 - PD7 normal port.
//TCCR1A  =  0x21;          //0b00100001 OC1B - PD4 compare match non inverting mode, down counting ,PWM 8-bit
TCCR1A |=  (1 << COM1B1);//Clear OC1B on compare match, set OC1B at BOTTOM, (non-inverting mode)
TCCR1A |=  (1 << WGM10);//PWM, Phase Correct, 8-bit
}
//--------------------------------------------------------------------------------------------------------------
//PD7 PWM and PC2 HIGH
void CH_AL(){
PORTC  = (0 << PC4)|(0 << PC3)|(1 << PC2);      //Set PC2 to HIGH and the rest to LOW
TCCR1A  =  0;            // OC1A/OC1B PD5/PD4 normal port
//TCCR2   =  0x81;           //OC2 - PD7compare match non inverting mode, down counting ,PWM 8-bit
TCCR2 |= (1 << COM21);//Clear OC2on compare match, set OC2 at BOTTOM, (non-inverting mode)
TCCR2 |= (1 << WGM20);//PWM, Phase Correct, 8-bit
}
//--------------------------------------------------------------------------------------------------------------
//PD7 PWM and PC3 HIGH
void CH_BL(){
PORTC  = (0 << PC4)|(1 << PC3)|(0 << PC2);       //Set PC3 to HIGH and the rest to LOW
TCCR1A  =  0;            // OC1A/OC1B PD5/PD4 normal port
//TCCR2   =  0x81;           //OC2 - PD7 compare match non inverting mode, down counting ,PWM 8-bit
TCCR2 |= (1 << COM21);//Clear OC2 on compare match, set OC2 at BOTTOM, (non-inverting mode)
TCCR2 |= (1 << WGM20);//PWM, Phase Correct, 8-bit
}
//--------------------------------------------------------------------------------------------------------------
#endif // COMMUTATION_H_ 
//--------------------------------------------------------------------------------------------------------------