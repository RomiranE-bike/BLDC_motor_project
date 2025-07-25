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
//---------------------------------------------------------------------------------------------------
#define action_DDR     DDRC
#define action_PORT    PORTC

#define BUZ_BIT  PC0// bit for buzzer output
#define LED_BIT  PC1// bit for LED output
//--------------------------------------------------------------------------------------------------------------
#define   f_beep   500    //beep frequency :1000 if CPU set 16MHZ. 500 if CPU set 4MHZ. 250  if CPU set 1MHZ
#define   d_beep   2000   //beep delay :3000us if CPU set 16MHZ.500us if CPU set 4MHZ. 250us if CPU set to 1MHZ
#define   d_blink  2000    //beep delay :3000 ms if CPU set 16MHZ.500ms if CPU set 4MHZ. 100 ms if CPU set to 1MHZ
//--------------------------------------------------------------------------------------------------------------
void beep();
void blink();
void alarm(void);

void startup_tone();
void tone_cyle(uint16_t ontime, uint16_t offtime);
void tone(uint8_t repetitions, uint8_t duration, uint16_t ontime, uint16_t offtime);

void beep_1KHZ (int milliseconds);
void beep_2KHZ (int milliseconds);
void beep_3KHZ (int milliseconds);
//---------------------------------------------------------------------------------------------------
// blinking routine//
void blink(){
	action_DDR=0xff;
	action_PORT=0x00;
	
	action_PORT  |= 1<<LED_BIT;// Toggle LED
	_delay_ms(d_blink);// Delay
	action_PORT  &=~ (1<<LED_BIT);// Toggle LED
	_delay_ms(d_blink);// Delay
}
//---------------------------------------------------------------------------------------------------
// Beeping routine//
void beep(){
	action_DDR=0xff;
	action_PORT=0x00;
	
	for(int i=0;i<f_beep;i++){// Loop
		action_PORT ^= 1<<BUZ_BIT;// Toggle BUZZER
		_delay_us(d_beep);// Delay
	}
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
void beep_1KHZ (int milliseconds){
	int x = 0;
	PORTC = (1 << PC6)+(0 << PC4)+(0 << PC2);         //Set PC6 to HIGH and the rest to LOW
	while (x < milliseconds)
	{
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(450);
		
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(450);
		x = x + 1;
	}
	PORTC = (0 << PC6)+(0 << PC4)+(0 << PC2);          //Set all to LOW
	PORTC = (0 << PC7)+(0 << PC5)+(0 << PC3);          //Set all to LOW
}
//--------------------------------------------------------------------------------------------------------------
void beep_2KHZ (int milliseconds){
	int x = 0;
	PORTC = (1 << PC6)+(0 << PC4)+(0 << PC2);           //Set PC4 to HIGH and the rest to LOW
	while (x < milliseconds)
	{
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(200);
		
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(200);

		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(200);
		
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(200);
		x = x + 1;
	}
	PORTC = (0 << PC6)+(0 << PC4)+(0 << PC2);          //Set all to LOW
	PORTC = (0 << PC7)+(0 << PC5)+(0 << PC3);          //Set all to LOW
}
//--------------------------------------------------------------------------------------------------------------
void beep_3KHZ (int milliseconds){
	int x = 0;
	PORTC = (1 << PC6)+(0 << PC4)+(0 << PC2);      //Set PC4 to HIGH and the rest to LOW
	while (x < milliseconds)
	{
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(150);
		
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(150);

		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(150);
		
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(150);

		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(1 << PC5)+(0 << PC3);      //Set  (AH) to HIGH (BH) to LOW
		_delay_us(150);
		
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(50);
		PORTC = (0 << PC7)+(0 << PC5)+(1 << PC3);      //Set  (BH) to HIGH (AH) to LOW
		_delay_us(150);
		x = x + 1;
	}
	PORTC = (0 << PC6)+(0 << PC4)+(0 << PC2);          //Set all to LOW
	PORTC = (0 << PC7)+(0 << PC5)+(0 << PC3);          //Set all to LOW
}
//--------------------------------------------------------------------------------------------------------------
//edited OK startup sound
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void startup_tone() {
	tone(1, 300, 5, 10);
	tone(1, 600, 10, 20);
	tone(1, 900, 20, 30);
	_delay_ms(10);
	tone(1, 900, 20, 30);
	tone(1, 600, 10, 20);
	tone(1, 300, 5, 10);
}
//-------------------------------------------------------------------------------------------------------------
void tone(uint8_t repetitions, uint8_t duration, uint16_t on_time, uint16_t off_time) {
	uint8_t i = 0;
	uint8_t q= 0;
	for (i=0; i < repetitions; i++){
		for (q=0; q < duration; q++){
			step1; tone_cyle(on_time, off_time);
			step2; tone_cyle(on_time, off_time);
			step3; tone_cyle(on_time, off_time);
			step4; tone_cyle(on_time, off_time);
			step5; tone_cyle(on_time, off_time);
			step6; tone_cyle(on_time, off_time);
		}
		_delay_ms(30);
	}
}
//-------------------------------------------------------------------------------------------------------------
void tone_cyle(uint16_t on_time, uint16_t off_time){
	
	uint16_t i=0;
	for(i=0; i < on_time; i++)
	_delay_us(1);
	    PORTC=0b10101000; //10-10-10 (U-V-W)// #define motor_break running break     
	    PORTC=0b00000000; //00-00-00 (U-V-W)//#define spin_free running free spin 
	    
	for(i=0; i < off_time; i++)
	_delay_us(1);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // BEEP_H_ 
//--------------------------------------------------------------------------------------------------------------