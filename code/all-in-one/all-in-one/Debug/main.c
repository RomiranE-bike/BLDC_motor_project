/*
 * all-in-one.c
 *
 * Created: 8/30/2024 14:41:48
 * Author : me
 */ 
//--------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "beep.h"
#include "io.h"
#include "LCD.h"
#include "menu.h"
#include "stepper.h"
#include "BLDC.h"
#include "NTC.h"
#include "timer.h"
//--------------------------------------------------------------------------------------------------
int main(void){  
		
		io_init();//initial Buttons and LEDs ports
		Reset();
		lcd_init();
		_delay_ms(200);
		Clear();
		beep();
		menu_init();//Initial menu and initial function
		timer0_init();//initial timer0 for key readings using overflow interrupts
		
		sei(); // Globally enable interrupts
		
		PORTC=0xff;
		_delay_ms(1000);
		PORTC=0x00;
		
		while(1){
			
			Line_stop();
			BLDC_off();
			//thermo_action();					
			//---------------------------------------------------------------------
			//set flag to 1
			//when button menu changes flag sets to 0
			Flag=1;
			//execute function that is pointed by FPtr
			if ((B_inpout_PIN &(1<<Enter_start))==0){
				_delay_ms(20);
				beep();
				FPtr();
				while((B_inpout_PIN &(1<<Enter_start))==0){}
			    }
				
			else if((B_inpout_PIN &(1<<Exit_stop))==0){
				_delay_ms(20);
				beep();
				menu_init();//Initial menu and initial function
				Line_stop();
				BLDC_off();
				
				while((B_inpout_PIN &(1<<Exit_stop))==0){}
				
			    }					
		}
		
	return 0;	
}
//--------------------------------------------------------------------------------------------------
