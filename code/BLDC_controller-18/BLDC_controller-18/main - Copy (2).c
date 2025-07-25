/*
 * BLDC_controller.c
 *
 * Created: 06/01/2024 08:10:08
 * edited : me
 */ 
//--------------------------------------------------------------------------------------------------------------
// Connecting a BLDC motor to an AVR atmega16 (without sensors) 
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
//--------------------------------------------------------------------------------------------------------------
#include "io.h"
#include "LCD.h"
#include "beep.h"
//#include "rpm.h"
//#include "interrupt.h"
//#include "button.h"
//#include "adc.h"
//#include "pwm.h"
//#include "commutation.h"
//--------------------------------------------------------------------------------------------------------------
volatile uint8_t    current_hall_position=0; //hall status
volatile uint8_t    last_hall_position=0; //hall status
volatile uint8_t    current_motor_position=0; //current motor position
volatile uint8_t    next_motor_position=1; //motor position (used to check spinning and direction), must be init_from 1 to 6
volatile uint8_t    right_position=0; //check  previous position , is it right?
//--------------------------------------------------------------------------------------------------------------
static   uint8_t      spin_pulse=0;
volatile uint8_t      spin_flag=0;

volatile uint32_t     spin_counter=0;
volatile uint32_t     cycle_timer=0;
volatile uint32_t     rpm=0;
//--------------------------------------------------------------------------------------------------------------
#define free_spin    PORTC=0b00000000; //00-00-00 (U-V-W)//running free spin (and red led on)
#define motor_break  PORTC=0b10101000; //10-10-10 (U-V-W)//running break     (and yellow  led on)

//define positions for running phases depending on hall sensor/position CW
#define CW101  step1//step2
#define CW100  step2//step1
#define CW110  step3//step6
#define CW010  step4//step5
#define CW011  step5//step4
#define CW001  step6//step3
//--------------------------------------------------------------------------------------------------------------
void  start_up_commutation();

void  Hall_position(); 
void  motor_spining();
void  motor_position();
void comparator();

void  timer0_init();
void  timer1_init();

void  rpm_init();
void  get_rpm();
//--------------------------------------------------------------------------------------------------------------
uint8_t main(void) {
		
    io_init();
    lcd_init();// Initialize LCD	
	beep();	
	Write_String(" BLDC motor ");
	//timer0_init();
	//timer1_init();
	
	while(1){
		
				   if((button_PIN & (1<<on_off_button))==0 ){
					   _delay_ms(20);
					   start_flag ^= 1;// on-off switch state
					   beep();				   
					   // Wait for the button to be released
					   while((button_PIN & (1<<on_off_button))==0 ){
					   } //end of while
				    } //end of if

				   if(start_flag == 1){
					   cli(); 
					   Set_Cursor(1,1);
					   Write_String("motor-->ON");
					   sei();
					   
					   Hall_position();
					   comparator();
					   //Clear();
					   motor_position();
					   //Clear();
					   motor_spining();
					   //Clear();
					   				   
						   	   
				       }
				   else {
					   cli();
					   Set_Cursor(1,1);
					   Write_String("motor-->OFF");
					   sei();
					   					   			   
					   PORTC=0x00;//motor off
					   blink();					   
				   }						
				 }//end of while
	
	return 0;
}
//--------------------------------------------------------------------------------------------------------------
//edited OK function call by output_status
void  start_up_commutation(){
	
	    uint8_t commut = 0;
		free_spin;
		_delay_us(15);
		//-------------------------------------------------------------------------------------------------------------
		for(commut=0; commut<6;commut++) {
			    current_motor_position = 1; //update last motor position
				CW101;//#define CW101  step	
				_delay_us(d_commut);		
			    current_motor_position = 2; //update last motor position
				CW100;//#define CW100  step
				_delay_us(d_commut);
			    current_motor_position = 3; //update last motor position
				CW110;//#define CW110  step
				_delay_us(d_commut);
			    current_motor_position = 4; //update last motor position
				CW010;//#define CW010  step
				_delay_us(d_commut);
			    current_motor_position = 5; //update last motor position
				CW011;//#define CW011  step
				_delay_us(d_commut);
			    current_motor_position = 6; //update last motor position
				CW001;//#define CW001  step
				_delay_us(d_commut);
		    }//end of for
	return 0;	
}
//--------------------------------------------------------------------------------------------------------------
//edited OK function call by check_motor_position
void  motor_spining(){
			
			if(current_motor_position == 1 && next_motor_position == 2){									   
					   CW101;//#define CW101  step1
			          }
			if(current_motor_position == 2 && next_motor_position == 3){	
					   CW100;//#define CW100  step2
			          }
			if(current_motor_position == 3 && next_motor_position == 4){		  
					   CW110;//#define CW110  step3 
			          }
			if(current_motor_position == 4 && next_motor_position == 5){		  
						CW010;//#define CW010  step4
			          }
            if(current_motor_position == 5 && next_motor_position == 6){
						CW011;//#define CW011  step5
			          }
			if(current_motor_position == 6 && next_motor_position == 1){
						CW001;//#define CW001  step6
			          }
					  
			spin_flag=1;
			spin_counter ++;
			/*
			cli();		  
			Set_Cursor(2,1);		  
			Write_String("spin_counter=");
			print_value(spin_counter);
			sei();	*/	  			   
}
//--------------------------------------------------------------------------------------------------------------
void  motor_position(){
	 	   	   
		   switch (current_hall_position){
			   case 1:	//	0b00000101
				   current_motor_position = 1;
				   next_motor_position = 2;
			   break;
			   case 2://0b00000100
				   current_motor_position = 2;
				   next_motor_position = 3;				   
			   break;
			   case 3://0b00000110
				   current_motor_position = 3;
				   next_motor_position = 4;
			   break;
			   case 4://0b00000010
				   current_motor_position = 4;
				   next_motor_position = 5;
			   break;
			   case 5://0b00000011
				   current_motor_position = 5;
				   next_motor_position = 6;
			   break;
			   case 6://0b00000001
				   current_motor_position = 6;
				   next_motor_position = 1;
			   break;		   
		   }//end of switch 
}
//--------------------------------------------------------------------------------------------------------------
void  Hall_position(){

		if((Hall_PIN & 1 << H1)) H1_status = 0b00000001;	
		else H1_status =0;
		if((Hall_PIN & 1 << H2)) H2_status =0b00000010;
		else H2_status =0;
		if((Hall_PIN & 1 << H3)) H3_status =0b00000100;
		else H3_status =0;	
		current_hall_position = H1_status + H2_status + H3_status;	
		if(current_hall_position == 0b00000000 || current_hall_position == 0b00000111) PORTC=0x00; 		
}
//--------------------------------------------------------------------------------------------------------------
void timer0_init(){
	
	TCNT0 = 0;// load TCNT0
	TCCR1B |= (1 << CS00); // No prescaling
	//TCCR0  |= (1 << CS02)|(1 << CS00); // Prescaler by 1024
	//TCCR1B   |= (1 << CS02);//prescaler 256
	//TCCR1B |= (1<<CS01) | (1<<CS00);  //prescaler 64
	//TCCR1B |= (1<<CS01);  //prescaler 8
	TIMSK    |= (1 << TOIE0); // Enable T0 overflow interrupt
/*	
CS02 CS01 CS00 Description
0 0 0 No clock source (Timer/Counter stopped).
0 0 1 clkI/O/(No prescaling)
0 1 0 clkI/O/8 (From prescaler)
0 1 1 clkI/O/64 (From prescaler)
1 0 0 clkI/O/256 (From prescaler)
1 0 1 clkI/O/1024 (From prescaler)
*/	
}
//--------------------------------------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect){
	
	cycle_timer++;		
}
//--------------------------------------------------------------------------------------------------------------
void timer1_init(){
	
	TCNT1 = 0;// load TCNT1
	//TCCR1B |= (1 << CS12)|(1 << CS10); // Prescaler by 1024
	//TCCR1B |= (1 << CS12);  //prescaler 256
	//TCCR1B |= (1<<CS11);  //prescaler 8
	//TCCR1B |= (1<<CS11) | (1<<CS10);  //prescaler 64
	TCCR1B   |= (1 << CS10); // No prescaling
	TIMSK |= (1 << TOIE1); //Enable T1 overflow interrupt
/*	
CS12 CS11 CS10 Description
0 0 0 No clock source (Timer/Counter stopped).
0 0 1 clkI/O/1 (No prescaling)
0 1 0 clkI/O/8 (From prescaler)
0 1 1 clkI/O/64 (From prescaler)
1 0 0 clkI/O/256 (From prescaler)
1 0 1 clkI/O/1024 (From prescaler)
*/	
}
//--------------------------------------------------------------------------------------------------------------
ISR(TIMER1_OVF_vect){
	
	TCCR0 = 0;            //stop timer0
	rpm = (cycle_timer * 256 + TCNT0) * (256/8) ; //[#define TIMER_STEP  256/8 micro sec]
	
	Set_Cursor(2,1);
	Write_String("cycle_timer=");
	print_value(cycle_timer);
	//Write_String("        ");
	
	Set_Cursor(2,1);
	Write_String(" RPM= ");
	print_value(rpm);
	Write_String("        ");
	
	    spin_counter = 0;
		cycle_timer = 0;      //clear overflow counter
		TCNT0 = 0;            //clear timer
		TCCR0 |= (1 << CS02)|(1 << CS00); //start timer0 Prescaler by 1024
		//TCCR0 |= (1<<CS02);  //start timer0 prescaler 256
		//TCCR0   |= (1 << CS00); //start timer0 No prescaling 
		//TCCR0 = (1<<CS01);    //start timer0

}
//--------------------------------------------------------------------------------------------------------------

void comparator(){
		

//ACSR |= (1<<ACIE);	                // Enable analog comparator interrupt
//ACSR |= (1<<ACIS1)| (1<<ACIS0);       // Comparator Interrupt on Rising Output Edge
//ACSR |= (1<<ACIS1)| (0<<ACIS0);       // Comparator Interrupt on Falling Output Edge

ADCSRA &= ~(1 << ADEN);	  // Disable the ADC module 
SFIOR |= (1<<ACME);	      // Enable analog comparator 
ACSR = 0x00;		      // Clear ACSR resister 

ADMUX = 0x05;		      // Select ADC pin as a -VE pin of comparator or AIN1 pin (SFIOR &=  ~(1 << ACME)) 
if( ACSR &(1 << ACO) ) A_status =1;  // Comparator output  Check ACO bit of ACSR register
else A_status = 0; 
Set_Cursor(2,1);
Write_String("A_status=");
print_value(A_status);
ACSR = 0x00;

ADMUX = 0x06;		      // Select ADC pin as a -VE pin of comparator or AIN1 pin (SFIOR &=  ~(1 << ACME)) 
if( ACSR &(1 << ACO) ) B_status =1;  // Comparator output  Check ACO bit of ACSR register
else B_status = 0;
Set_Cursor(2,1);
Write_String("B_status=");
print_value(B_status);
ACSR = 0x00;

ADMUX = 0x07;		      // Select ADC pin as a -VE pin of comparator or AIN1 pin (SFIOR &=  ~(1 << ACME)) 
if( ACSR &(1 << ACO) ) C_status =1;  // Comparator output  Check ACO bit of ACSR register
else C_status = 0;
Set_Cursor(2,1);
Write_String("C_status=");
print_value(C_status);
ACSR = 0x00;
		
	
}



