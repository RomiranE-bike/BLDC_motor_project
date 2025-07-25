/*
 * commutation.h
 *
 * Created: 6/1/2024 14:22:56
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
/*
=====
CW
=====
H H H - UU VV WW
1 2 3 - HL HL HL
----------------
1 0 1 - 10 00 01
1 0 0 - 10 01 00
1 1 0 - 00 01 10
0 1 0 - 01 00 10
0 1 1 - 01 10 00
0 0 1 - 00 10 01

=====
CCW
=====
H H H - UU VV WW
1 2 3 - HL HL HL
----------------
1 0 1 - 10 01 00
1 0 0 - 00 01 10
1 1 0 - 01 00 10
0 1 0 - 01 10 00
0 1 1 - 00 10 01
0 0 1 - 10 00 01
*/
//--------------------------------------------------------------------------------------------------------------
#ifndef COMMUTATION_H_
#define COMMUTATION_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include "io.h"
#include "pwm.h"
//--------------------------------------------------------------------------------------------------------------
void commutation(unsigned char startup);
void BLDC_turning_120();
void BLDC_turning_180();
void Hall_status();
void BLDC_step();
//--------------------------------------------------------------------------------------------------------------
void Hall_status(){
	
	if((Hall_PIN & 1 << H1)) H1_status =0b00000001 ;
	else H1_status =0;
	if((Hall_PIN & 1 << H2)) H2_status =0b00000010;
	else H2_status =0;
	if((Hall_PIN & 1 << H3)) H3_status =0b00000100 ;
	else H3_status =0;
	step = H1_status + H2_status + H3_status;
	//if(step==0b00000000 || step==0b00000111)PORTC=0x00;
}
//--------------------------------------------------------------------------------------------------------------
void BLDC_step(){
	
	Hall_status();
	while(step==0b00000111 || step==0b00000010 || step==0b00000000){//Hall_status=010
		AH_BL();
		Hall_status();
		if(step==0b00000110){
		    break;	
	        Write_String("   step1");
		   }		
	      }
	while(step==0b00000110){//Hall_status=011
		AH_CL();
		Hall_status();
	    if(step==0b00000100){
			break;		
		    Write_String("   step2");
	       }
	      }
	while(step==0b00000100){//Hall_status=001
		BH_CL();
		Hall_status();
	    if(step==0b00000101){
			break;		
		    Write_String("   step3");
	     }
	    }
	while(step==0b00000101){//Hall_status=101
		BH_AL();
		Hall_status();
	    if(step==0b00000001){
            break;
		    Write_String("   step4");
	      }
	     }
	while(step==0b00000001){//Hall_status=100
		CH_AL();
		Hall_status();
	    if(step==0b00000011){
		    break;
		    Write_String("   step5");
	       }
	      }
	while(step==0b00000011){//Hall_status=110
		CH_BL();
		Hall_status();
	    if(step==0b00000111 || step==0b00000010 || step==0b00000000){
		    break;
		    Write_String("   step6");
	       }
	      }
	//else{
		//PORTC=0x00;
		//blink();
	   //}
	
}
//--------------------------------------------------------------------------------------------------------------
void BLDC_step1(){
	
	Hall_status();

	if( step==0b00000010) {
		CH_BL();
		//Write_String("   step1");//Hall_status=010
	}
	else if(step==0b00000110){
		CH_AL();
		//Write_String("   step2");//Hall_status=110
	}
	else if(step==0b00000100){
		BH_AL();
		//Write_String("   step3");//Hall_status=100
	}
	else if(step==0b00000101){
		BH_CL();
		//Write_String("   step4");//Hall_status=101
	}
	else if(step==0b00000001){
		AH_CL();
		//Write_String("   step5");//Hall_status=001
	}
	else if(step==0b00000011){
		AH_BL();
		//Write_String("   step6");//Hall_status=011
	}
	else{
		PORTC=0x00;
		//blink();
	}
	
}
//--------------------------------------------------------------------------------------------------------------
void BLDC_step2(){
	
	Hall_status();
	
	if( step==0b00000010) {
		AH_BL();
		//Write_String("   step1");//Hall_status=010
	}
	else if(step==0b00000011){
		AH_CL();
		//Write_String("   step2");//Hall_status=011
	}
	else if(step==0b00000001){
		BH_CL();
		//Write_String("   step3");//Hall_status=001
	}
	else if(step==0b00000101){
		BH_AL();
		//Write_String("   step4");//Hall_status=101
	}
	else if(step==0b00000100){
		CH_AL();
		//Write_String("   step5");//Hall_status=100
	}
	else if(step==0b00000110){
		CH_BL();
		//Write_String("   step6");//Hall_status=110
	}
	else{
		PORTC=0x00;
		//blink();
	}
	
}
//--------------------------------------------------------------------------------------------------------------
// Motor winding switching function
void commutation(unsigned char startup){
	
	switch (sequence_step){
		case (0):
		if(!(ACSR & (1 << ACO)) || startup){ // Comparator output SENSE_UVW
			AH_BL(); ADMUX = 4; // Back EMF input phase W //SENSE_W;
			_delay_ms(d_commut);
			sequence_step = 1; // Next step
			TCNT0 = 0; // Reset counter T0
		}
		break;
		case (1):
		if((ACSR & (1 << ACO)) || startup){ // Comparator output SENSE_UVW
			AH_CL(); ADMUX = 3; // Back EMF input phase V //SENSE_V;
			_delay_ms(d_commut);
			sequence_step = 2;
			TCNT0 = 0; // Reset counter T0
		}
		break;
		case (2):
		if(!(ACSR & (1 << ACO)) || startup){ // Comparator output SENSE_UVW
			BH_CL(); ADMUX = 2; // Back EMF input phase U //SENSE_U;
			_delay_ms(d_commut);
			sequence_step = 3;
			TCNT0 = 0; // Reset counter T0
		}
		break;
		
		case (3):
		if((ACSR & (1 << ACO)) || startup){ // Comparator output SENSE_UVW
			BH_AL(); ADMUX = 4; // Back EMF input phase W //SENSE_W;
			_delay_ms(d_commut);
			sequence_step = 4;
			TCNT0 = 0; // Reset counter T0
		}
		break;
		
		case (4):
		if(!(ACSR & (1 << ACO)) || startup){ // Comparator output SENSE_UVW
			CH_AL(); ADMUX = 3; // Back EMF input phase V //SENSE_V;
			_delay_ms(d_commut);
			sequence_step = 5;
			TCNT0 = 0; // Reset counter T0
		}
		break;
		
		case (5):
		if((ACSR & (1 << ACO)) || startup){ // Comparator output SENSE_UVW
			CH_BL(); ADMUX = 2; // Back EMF input phase U //SENSE_U;
			_delay_ms(d_commut);
			sequence_step = 0;
			TCNT0 = 0; // Reset counter T0
		}
		break;
	}
}
//--------------------------------------------------------------------------------------------------------------
//120 degree commutation
void BLDC_turning_120(){
	switch(sequence_step){
		case 0:
		AH_BL();
		BEMF_C_RISING();
		break;
		case 1:
		AH_CL();
		BEMF_B_FALLING();
		break;
		case 2:
		BH_CL();
		BEMF_A_RISING();
		break;
		case 3:
		BH_AL();
		BEMF_C_FALLING();
		break;
		case 4:
		CH_AL();
		BEMF_B_RISING();
		break;
		case 5:
		CH_BL();
		BEMF_A_FALLING();
		break;
	}
}//end of BLDC_move
//--------------------------------------------------------------------------------------------------------------
//180 degree commutation
void BLDC_turning_180(){
	switch(sequence_step){
		case 0:
		AH_BL_CH();
		BEMF_A_RISING();
		BEMF_B_FALLING();
		BEMF_C_RISING();		
		break;
		case 1:
		AH_BL_CL();
		BEMF_A_RISING();
		BEMF_B_FALLING();
		BEMF_C_FALLING();
		break;
		case 2:
		AH_BH_CL();
		BEMF_A_RISING();
		BEMF_B_RISING();
		BEMF_C_FALLING();
		break;
		case 3:
		AL_BH_CL();
		BEMF_A_FALLING();
		BEMF_B_RISING();
		BEMF_C_FALLING();
		break;
		case 4:
		AL_BH_CH();
		BEMF_A_FALLING();
		BEMF_B_RISING();
		BEMF_C_RISING();
		break;
		case 5:
		AL_BL_CH();
		BEMF_A_FALLING();
		BEMF_B_FALLING();
		BEMF_C_RISING();		
		break;
	}
}//end of BLDC_move
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////////// BEMF pulseC////////////////////////////////////////////////
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
///////////////////////////////////////////120 degree commutation///////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------
void AH_BL(){
	//PORTC   = 0x00;
	//PORTC   |= (1 << AH)|(1 << BL);
	PORTC=0b10010000;
}
//--------------------------------------------------------------------------------------------------------------
void AH_CL(){
	//PORTC   = 0x00;
	//PORTC   |= (1 << AH)|(1 << CL);
	PORTC=0b10000100;
}
//--------------------------------------------------------------------------------------------------------------
void BH_CL(){
	//PORTC   = 0x00;
	//PORTC   |= (1 << BH)|(1 << CL);
	PORTC=0b00100100;
}
//--------------------------------------------------------------------------------------------------------------
void BH_AL(){
	//PORTC   = 0x00;
	//PORTC   |= (1 << BH)|(1 << AL);
	PORTC=0b01100000;
}
//--------------------------------------------------------------------------------------------------------------
void CH_AL(){
	//PORTC   = 0x00;
	//PORTC   |= (1 << CH)|(1 << AL);
	PORTC=0b01001000;
}
//--------------------------------------------------------------------------------------------------------------
void CH_BL(){
	//PORTC   = 0x00;
	//PORTC   |= (1 << CH)|(1 << BL);
	PORTC=0b00011000;
}
//--------------------------------------------------------------------------------------------------------------
//////////////////////////////////////////////180 degree commutation//////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------------------------
void AH_BL_CH(){
	PORTC   = 0x00;
	PORTC   |= (1 << AH)|(1 << BL)|(1 << CH);
}
//--------------------------------------------------------------------------------------------------------------
void AH_BL_CL(){
	PORTC   = 0x00;
	PORTC   |= (1 << AH)|(1 << BL)|(1 << CL);
}
//--------------------------------------------------------------------------------------------------------------
void AH_BH_CL(){
	PORTC   = 0x00;
	PORTC   |= (1 << AH)|(1 << BH)|(1 << CL);
}
//--------------------------------------------------------------------------------------------------------------
void AL_BH_CL(){
	PORTC   = 0x00;
	PORTC   |= (1 << AL)|(1 << BH)|(1 << CL);
}
//--------------------------------------------------------------------------------------------------------------
void AL_BH_CH(){
	PORTC   = 0x00;
	PORTC   |= (1 << AL)|(1 << BH)|(1 << CH);
}
//--------------------------------------------------------------------------------------------------------------
void AL_BL_CH(){
	PORTC   = 0x00;
	PORTC   |= (1 << AL)|(1 << BL)|(1 << CH);
}
//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------

#endif // COMMUTATION_H_ 
//--------------------------------------------------------------------------------------------------------------
