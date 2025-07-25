/*
 * button.h
 *
 * Created: 6/11/2024 20:46:11
 *  Author: me
 */ 
//--------------------------------------------------------------------------------------------------------------
#ifndef BUTTON_H_
#define BUTTON_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "io.h"
#include "LCD.h"
//--------------------------------------------------------------------------------------------------------------
//     ___/ _______to input pin
//     |       |
//   ------    |__/\/\/\__to VCC
//     --
void button_hold(){
	
	if((button_PIN & (1<<on_off_button))==0 ){
			_delay_ms(20);
			start_flag ^= 1;// on-off switch state
			beep();
			// Wait for the button to be released
			while((button_PIN & (1<<on_off_button))==0 ){			
				} //end of while
			  } //end of if
	
			if(start_flag){
				Clear();
				Write_String("motor on");//display running
				_delay_ms(1000);
				//doing action
				//new_commut(start_flag);
		
					if((button_PIN & (1<<stop_button))==0){			
						Clear();
						Write_String("pwm_set");//display running
						_delay_ms(100);	//other action
					} //end of if
			   }//end of if
		else {
			start_flag=0;
			Clear();
			Write_String("motor off");		
			PORTC=0x00;
			blink();
			//doing action
	    	}//end of else	
}
//--------------------------------------------------------------------------------------------------------------
//     ___/ ___ to input pin
//     |
//   ------
//     --
void button_push(){
									
			if(! (button_PIN & 1<<on_off_button) ){
				start_flag = 1;
				beep();
				while(start_flag == 1){
					Clear();
					Write_String(" run");//display running												
					if(! (button_PIN & 1<<stop_button) ){
							beep();
							start_flag = 0;
							break;												
							} //end of if												
					} //end of while
					Clear();
					Write_String(" stop");//display stop
					_delay_ms(1000);
				} //end of if
			else{
				blink();//doing action
				Clear();
				Write_String(" waiting");//display waiting
				//other action
				} //end of else				
}
//--------------------------------------------------------------------------------------------------------------
#endif // BUTTON_H_ 
//--------------------------------------------------------------------------------------------------------------