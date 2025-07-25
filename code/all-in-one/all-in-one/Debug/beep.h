/*
 * beep.h
 *
 * Created: 6/1/2024 15:31:26
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
#ifndef BEEP_H_
#define BEEP_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
//--------------------------------------------------------------------------------------------------------------
#define        d_blink  2000     // beep delay :2000 ms if clock set to 8MHZ /100 ms if clock set to 1MHZ
#define        d_beep   2000     // beep delay :2000us if clock set to 8MHZ  /250us if clock set to 1MHZ
#define        f_beep   500     // beep frequency :500 cycle  if  clock set to 8MHZ/ 250 cycle if clock set to 1MHZ
//--------------------------------------------------------------------------------------------------------------
void beep();
void blink();
void alarm(void);
//---------------------------------------------------------------------------------------------------
// blinking routine
void blink(){
	action_port |= 1 << LED;       // Toggle LED
	_delay_ms(d_blink);            // Delay
	action_port &= ~(1 << LED);    // Toggle LED
	_delay_ms(d_blink);            // Delay
}
//---------------------------------------------------------------------------------------------------
// Beeping routine
void beep(){

	int i;
	for(i=0;i<f_beep;i++){           // Loop
		action_port |= 1 << BUZ;     // Toggle BUZZER
		_delay_us(d_beep);           // Delay
		action_port &= ~(1 << BUZ);  // Toggle BUZZER
		_delay_us(d_beep);           // Delay
	};
}
//---------------------------------------------------------------------------------------------------
void alarm(void){
	for(int i=0;i<50;i++){
		beep();
		blink();
		_delay_ms(10);
	}
}
//-------------------------------------------------------------------------------------------------------------
#endif // BEEP_H_ 
//--------------------------------------------------------------------------------------------------------------