/*
 * BLDC_controller.c
 *
 * Created: 07/16/2024 22:20
 * edited : me
 */ 
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
//----------------------------------------------------------------------------------------------------------------
uint16_t    sensor_value;
uint16_t    T1_flag,T1_sens,T1_limit=70;
uint16_t    sens_value_1;

//----------------------------------------------------------------------------------------------------------------

#define PHASE_ALL_OFF  PORTC =0x00;
#define START_PWM 50 // Minimum PWM at startup
//----------------------------------------------------------------------------------------------------------------
unsigned char  position=0;
volatile unsigned char sequence_step = 0;
volatile unsigned char rotor_run = 0; // Back EMF pulse counter
unsigned int  soft_start =5000 ;
//----------------------------------------------------------------------------------------------------------------
// ADC sample at specified channel, return 10-bit result
uint16_t adc_read(uint16_t channel){
	
	ADMUX |= (channel & 0b0000111);//set ADC channel : channel must be 0 to 7  (ADC0....ADC7)
	//ADMUX |= (1<<REFS1) | (1<<REFS0);//Internal 2.56V Voltage Reference with external capacitor at AREF pin
	ADMUX |= (1<<REFS0);//AVCC reference voltage  with external capacitor at AREF pin
	
	ADCSRA |=(0 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);// prescalar: 8 => 1MHz/8 = 128kHz
	//ADCSRA |=(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);// prescalar: 128 => 1MHz/128 = 8kHz
	
	ADCSRA|= (1<<ADEN);//ADC Conversion enable
	//ADCSRA|= (1<<ADFR);//ADC Free Running Select
	//ADCSRA|= (1<<ADIE);//ADC Interrupt Enable
	ADCSRA|= (1<<ADSC);//ADC Start Conversion
	while(!(ADCSRA & (1<<ADIF)));// waiting for ADIF, conversion complete,Clear flag
	ADCSRA|=(1<<ADIF); // clearing of ADIF, it is done by writing 1 to it
	return (ADC);// return results
}
//----------------------------------------------------------------------------------------------------------------
uint16_t sensor_read(uint16_t sensor){

	unsigned char sample;
	sensor_value = adc_read(sensor);//analog to digital conversion
	for(sample=0;sample<16;sample++){
		sensor_value += adc_read(sensor);// read ADC samples
	}
	sensor_value >>= 4;// take average of the 16 samples 'adc_val /= 16'  0b 1111 1111 >> 4 = 0b 0000 1111
	return  sensor_value;
}
//----------------------------------------------------------------------------------------------------------------
void thermo_action(void){
	
	sens_value_1 = sensor_read(7);
	T1_sens = NTC_function_1(sens_value_1);
	Set_Cursor(2,1);
	Write_String("thermo=");
	print_value(T1_sens);
	
		while(T1_sens > T1_limit){
			PORTC = 0x00;//port off if T > 70
			beep();
			R_LED_ON;
			sens_value_1 = sensor_read(7);
			T1_sens = NTC_function_1(sens_value_1);
			if(T1_sens < 50)break;			
		}//end of while
		R_LED_OFF;
}
//----------------------------------------------------------------------------------------------------------------
// Rotate clockwise
void commutation(char startup){
	
	switch (sequence_step){
		
		case (0):
		if(!(ACSR&(1 << ACO)) || startup){
			step1;
			ADMUX = 2; // Activate the W phase input
			sequence_step = 1; // Next step
			TCNT0 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		    }
		break;	
		case (1):
		if((ACSR&(1 << ACO)) || startup){
			step2;
			ADMUX = 1; // Activate phase V input
			sequence_step = 2;
			TCNT0 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		   }
		break;
		
		case (2):
		if(!(ACSR &(1 << ACO)) || startup){
			step3;
			ADMUX = 0; // Activate phase U input
			sequence_step = 3;
			TCNT0 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		  }
		break;
		
		case (3):
		if((ACSR &(1 << ACO)) || startup){
			step4;
			ADMUX = 2; // Activate the W phase input
			sequence_step = 4;
			TCNT0 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		   }
		break;
		
		case (4):
		if(!(ACSR &(1 << ACO)) || startup){
			step5;
			ADMUX = 1; // Activate phase V input
			sequence_step = 5;
			TCNT0 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		   }
		break;
		
		case(5):
		if((ACSR &(1 << ACO)) || startup){
			step6;
			ADMUX = 0; // Activate phase U input
			sequence_step = 0;
			TCNT0 = 0; // Reset counter T0
			TCNT2 = 0; // Reset counter T2
		   }
		break;
	}
}
//----------------------------------------------------------------------------------------------------------------
// Comparator interrupt handler. Back EMF detector
ISR(ANA_COMP_vect){
	
}
//----------------------------------------------------------------------------------------------------------------
// T0 overflow interrupt handler. Motor operation without back EMF signals
ISR(TIMER0_OVF_vect){
	
	commutation(1); // If the interrupt is triggered, there are missing back-EMF pulses
	rotor_run = 0;   // Reset the pulse counter
	
}
//----------------------------------------------------------------------------------------------------------------
// T2 overflow interrupt handler. Motor operation with back EMF signals
ISR(TIMER2_OVF_vect){
	
	if(rotor_run == 200) commutation(0);// if the interrupt is triggered, there are missing back-EMF pulses
	rotor_run++;
	if(rotor_run > 200){
		rotor_run = 200;
	}		
}
//----------------------------------------------------------------------------------------------------------------
int main( void ){
	
    
	io_init();
	lcd_init();// Initialize LCD
	beep();
	
	Clear();
	Set_Cursor(1,1);
	Write_String(" BLDC << setup");
	_delay_ms(5000);

	PHASE_ALL_OFF; // Turn off all phases
	SFIOR |= (1 << ACME); // Connect the negative input of the comparator to the output of the ADC multiplexer
	ACSR |=(1 << ACI);         // Clear flag comparator interrupt
	
	// T0 - for starting and operating the engine without back EMF signals
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
	TCCR0 = 0;
	TCCR0 = 0x04;// Prescaler by 256
	TIMSK |= (1 << TOIE0); // Enable T0 overflow interrupt
	
	
	// T2 - for starting and operating the engine without back EMF signals
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
	TCCR2 = 0;
	TCCR2 = 0x01;// Prescaler by 1
	TIMSK |= (1 << TOIE2); // Enable T0 overflow interrupt
		
	sei(); // Globally enable interrupts
	
	while(1){
		
				thermo_action();
				//////////////////////////// Motor start///////////////////////
				while((button_PIN & (1<< startup_button))==0 ){
					startup = 2200;
						while(startup > 100){
							_delay_ms(100);
							commutation(1);
							sequence_step++;
							sequence_step %= 6;
							startup = startup - 20;
						}//end of while
					ACSR |= (1 << ACIE);           // Enable analog comparator interrupt
					}//end of while
				 
			if((button_PIN & (1<<on_off_button))==0 ){
				_delay_ms(20);
				start_flag ^= 1;// on-off switch state
				beep();
				Clear();
				// Wait for the button to be released
				while((button_PIN & (1<<on_off_button))==0 ){} //end of while
			    } //end of if

				if(start_flag == 1){
						
						position = ADMUX;	// save position to the saved buffer				
						ACSR &= 0x00;		// Clear ACSR register
						ADCSRA &= ~(1 << ADEN);// Turn off the ADC
						SFIOR  |= (1 << ACME); // Connect the negative input of the comparator to the output of the ADC multiplexer
						ADMUX = position;// set position with the saved buffer
						sei();
						ACSR   |= (1 << ACIE);  // Enable interrupt from comparator
						TIMSK  |= (1 << TOIE0); // Enable T0 overflow interrupt
			        
						Set_Cursor(1,1);
						Write_String(" motor is ON");
						
						
						/*
						if(rotor_run==200){
							G_LED_ON;
							R_LED_OFF;
							Set_Cursor(2,1);
							Write_String(" motor spinning");
						}
						else{
							R_LED_ON;
							G_LED_OFF;
							Set_Cursor(2,1);
							Write_String(" STRTUP running");
						}
						*/
						
										
		
				 }//end of if
				 else {
					 
					 PHASE_ALL_OFF;//motor off
					 ACSR  &= ~(1 << ACIE); // Disable the comparator interrupt
					 TIMSK &= ~(1 << TOIE0); // Disable T0 overflow interrupt
					 					 
					 Set_Cursor(1,1);
					 Write_String(" motor is OFF");
					 
					 blink();
				 }//end of else
			 }//end of while
}
//----------------------------------------------------------------------------------------------------------------

