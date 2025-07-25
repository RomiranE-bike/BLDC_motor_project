//--------------------------------------------------------------------------------------------------------------
#ifndef BLDCSENSORED_H_
#define BLDCSENSORED_H_
//--------------------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
#include "io.h"
#include "beep.h"
//--------------------------------------------------------------------------------------------------------------
//running operations can also be implemented in 1 instruction, for example setting all a motor output on the sample port,
// a running instruction can be just a PORT setting
#define spin_free    PORTC=0b00000000; //00-00-00 (U-V-W)//running free spin (and red led on)
#define motor_break  PORTC=0b10101000; //10-10-10 (U-V-W)//running break     (and yellow  led on)
//running phases
//--------------------------------------------------------------------------------------------------------------
//define positions for running phases depending on hall sensor/position CW
#define CW101  step1//step2
#define CW100  step2//step1
#define CW110  step3//step6
#define CW010  step4//step5
#define CW011  step5//step4
#define CW001  step6//step3
//define positions for running phases depending on hall sensor/position CCW
#define CCW101  step6//step1
#define CCW100  step5//step6
#define CCW110  step4//step5
#define CCW010  step3//step4
#define CCW011  step2//step3
#define CCW001  step1//step2
//--------------------------------------------------------------------------------------------------------------
//startup delay, the startup delay for every step is given by startup delay array*STARTUPDELAYMULT
#define startup_delays {20, 15, 10, 8, 7, 6, 5}
#define startup_commut    6 //number of startup commutations
#define startup_delay_multiplier       5 //startup delay multiplier
//--------------------------------------------------------------------------------------------------------------
//stop steps
#define stop_position_steps      500 //number of same position step to declare that motor not spinning
//--------------------------------------------------------------------------------------------------------------
//setup speed
#define speed_MIN       600 //define the minimum speed (from 0 to 0xFFFF), number of cycle to stop stop the motor
//--------------------------------------------------------------------------------------------------------------
//freq = FCPU/1+(prescaler*ICR1)
//note: ICR1 can not be small, because we have some commands to execute during the interrupt routine
//20000Hz = 8000000Hz/(1+(1*399))
#define TIMER1_ICR1         399
#define TIMER1_PRESCALER    (1<<CS10) //prescaler
//--------------------------------------------------------------------------------------------------------------
//direction definitions
#define direction_CW   1
#define direction_CCW  1
//--------------------------------------------------------------------------------------------------------------
//spin direction definitions
#define spin_NONE   1
#define spin_CW     1
#define spin_CCW    1
//--------------------------------------------------------------------------------------------------------------
volatile uint8_t    motor_direction = direction_CW;//current direction
volatile uint8_t    spin_direction = spin_CW;//current spinning direction
volatile uint8_t    motor_flag = 0;//current running status
volatile uint16_t   speed_value = 0;//current speed
//-------------------------------------------------------------------------------------------------------------
//motor position debug variables
volatile uint8_t    debug_state = 0;
volatile uint8_t    position_error = 0;
volatile uint8_t    last_position_error = 0;

volatile uint8_t    right_position  = 0; //check  previous position , is it right?	
//-------------------------------------------------------------------------------------------------------------
static uint8_t    current_motor_position; //current motor position
static uint8_t    last_motor_position; //motor position (used to check spinning and direction), must be init_from 1 to 6
static uint8_t    current_hall_status; //current hall status
static uint8_t    last_hall_status; //last hall status
static uint16_t   current_spin_direction; //current spin direction
static uint16_t   last_spin_direction; //last spin direction counter
static uint16_t   speed_counter; //counter for the PWM emission
static uint8_t    PWM_controller; //PWM controller
static uint8_t    output_controller; //output to  _controller
static uint8_t    startup_sequence; //emit startup sequence
static uint8_t    wrong_spin; //wrong spin detector
static uint8_t    wrong_spin_status; //emit a wrong status, i.e. break motor
static uint16_t   stop_position_counter; //counter for the stop position
static uint16_t   spin_direction_counter; //counter for the spin direction

//-------------------------------------------------------------------------------------------------------------
//functions
void          bldc_init();
uint16_t      set_speed();//edited OK function call by bldc_init  and running

//-------------------------------------------------------------------------------------------------------------
unsigned int    ADC_value = 0;
unsigned int    ADC_init(unsigned int  channel);
unsigned int    ADC_read(unsigned int  channel);
void            print_value(unsigned char value);
//-------------------------------------------------------------------------------------------------------------
//void  speed_controller();//edited OK function call by (TIMER1 over flow vector)
void  check_motor_not_spinnig();//edited OK function call by (TIMER1 over flow vector)
void  update_spin_direction();//edited OK function call by (TIMER1 over flow vector)
void  check_motor_position();//edited OK function call by (TIMER1 over flow vector)
void  output_status();//edited OK function call by (TIMER1 over flow vector)

void  Hall_status_position();//edited OK function call by check_motor_position
void  wrong_spin_detector();//edited OK function call by check_motor_position
void  startup_commutation();//edited OK function call by output_status
void  hall_status_output();//edited OK function call by output_status
//-------------------------------------------------------------------------------------------------------------
void  runing();
//--------------------------------------------------------------------------------------------------------------
//edited OK init_bldc ports and variables
void bldc_init() {
	cli();

	motor_flag = 0;//stop the motor spinning
	set_speed();
	
	motor_break;
	_delay_ms(5);	
	spin_free;
	//beep();	
	startup_tone();//startup sound
	
/*
	ICR1 = TIMER1_ICR1; //ICR1
	TCCR1A = (1 << WGM11); //fast pwm_mode
	TCCR1B = (1 << WGM13) | (1 << WGM12);
	TCCR1B |= TIMER1_PRESCALER; //prescaler
	TIMSK |= (1<<TOIE1); //enable timer1
 */
///*
   // T0 - for starting and operating the engine 
   TCNT0 = 0;// load TCNT0
   //TCCR0 |= (1 << CS02)|(1 << CS00); // Prescaler by 1024
   TCCR0   |= (1 << CS00); // No prescaling
   TIMSK |= (1 << TOIE0); // Enable T0 overflow interrupt
  // */
	sei();
}
//-------------------------------------------------------------------------------------------------------------
void runing(){
	
	
	uint16_t current_direction = 0;
	uint16_t last_direction = 0;
		
	
		if(current_spin_direction==spin_NONE){
			Clear();
			Write_String(" spin_NONE");
			_delay_ms(show_delay);
			}
		if(current_spin_direction==spin_CW){
			Clear();
			Write_String(" spin_CW");
			_delay_ms(show_delay);
			}
		if(current_spin_direction==spin_CCW){
			Clear();
			Write_String(" spin_CCW");
			_delay_ms(show_delay);
			}
		if(position_error != 0) {
				Clear();
				Write_String("position_error");
				_delay_ms(show_delay);
				position_error = 0;
			   }
		   
        set_speed();
	 
		//set  direction
		if(button_PIN & (1 << direction_button)){
			Clear();
			Write_String("direction = CW");
			_delay_ms(show_delay);
			current_direction = direction_CW;
		    }
		else {
			Clear();
			Write_String("direction = CCW");
			_delay_ms(show_delay);
			current_direction = direction_CCW;
		    }//set for direction
			
		//direction changed					
		if(current_direction != last_direction) { 
			if(current_direction == direction_CW) {
				Clear();
				Write_String("changed CW");
				_delay_ms(show_delay);
				} 
				else if(current_direction == direction_CCW) {
				Clear();
				Write_String("changed CCW");
				_delay_ms(show_delay);
			    }
			last_direction = current_direction;
			if(current_direction == direction_CW || current_direction == direction_CCW) {
				cli();
				motor_direction = current_direction;
				sei();
			    }		    
			}//direction changed	
}
//--------------------------------------------------------------------------------------------------------------
//TIMER over flow vector
ISR(TIMER0_OVF_vect) {
	/*
	current_motor_position = 0; //current motor position
	last_motor_position = 1; //motor position (used to check spinning and direction), must be init_from 1 to 6
	current_hall_status = 0; //current hall status
	last_hall_status = 0; //last hall status
	current_spin_direction = 0; //current spin direction
	last_spin_direction = 0; //last spin direction counter
	speed_counter = 0; //counter for the PWM emission
	PWM_controller = 0; //PWM controller
	output_controller = 0; //output to  _controller
	startup_sequence = 0; //emit startup sequence
	wrong_spin = 0; //wrong spin detector
	wrong_spin_status = 0; //emit a wrong status, i.e. break motor
	stop_position_counter = 0; //counter for the stop position
	spin_direction_counter = 0; //counter for the spin direction
	
	speed_controller();
	check_motor_not_spinnig();
	update_spin_direction();
	check_motor_position();
	output_status();
   */ 
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by (TIMER1 over flow vector)
void  check_motor_not_spinnig(){
	
	//define stop_position_steps 500 //number of same position step  when motor not spinning
	
	if(stop_position_counter > stop_position_steps){
		stop_position_counter = 0; //reset stop position counter
		if(motor_flag){ //running status
			//if is not spinning emit a startup status
			output_controller = 1;
			startup_sequence  = 1;
		}
		current_spin_direction = spin_NONE;
		Clear();
		Write_String("spin_NONE");
		_delay_ms(show_delay);
	}
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by (TIMER1 over flow vector)
void  update_spin_direction(){
	
	if(last_spin_direction != current_spin_direction){
		spin_direction_counter++;
		if(spin_direction_counter > 200){ //update direction if we are spinning many times this way
			spin_direction_counter = 0;
			last_spin_direction = current_spin_direction;
			spin_direction = current_spin_direction;//spin direction definitions{spin_NONE  0,spin_CW  1,spin_CCW  2}
			Clear();
			Write_String("spin_status = ");print_value(spin_direction);
			_delay_ms(show_delay);
		    }
	    }
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by (TIMER1 over flow vector)
void  check_motor_position(){
	
	//get current hall status
	//-------------------------------------------------------------------------------------------------------------
	current_hall_status = ((!((Hall_PIN &(1<<H1))>>H1)<<2)|(!((Hall_PIN &(1<<H2))>>H2)<<1)|(!((Hall_PIN &(1<<H3))>>H3)<<0)) & 0b00000111;
	Clear();
	Write_String("hall_status = ");print_value(current_hall_status);
	_delay_ms(show_delay);
	
    //hall_status other format	
	/*
	char H3_status,H2_status,H1_status;
	if(H1_PIN &(1<<H1)) H1_status =0b00000001 ;
	else H1_status =0;
	if(H2_PIN &(1<<H2)) H2_status =0b00000010;
	else H2_status =0;
	if(H3_PIN &(1<<H3)) H3_status =0b00000100 ;
	else H3_status =0;
	
	hall_status = H3_status + H2_status + H1_status;
	Clear();
	Write_String("hall_status=");print_value(hall_status);
	_delay_ms(show_delay);
	*/
	
	if(current_hall_status != last_hall_status && !startup_sequence){
		last_hall_status = current_hall_status;
		stop_position_counter = 0; //reset stop position counter		
		spin_free;
        Hall_status_position();
		wrong_spin_detector();		
	     }
	else{//hall sensor in the previous position as before		
		if(stop_position_counter <= stop_position_steps)//define stop_position_steps 500 //number of same position step  when motor not spinning
		   stop_position_counter++; //increment stop position counter
	      }
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by check_motor_position
void  Hall_status_position(){		
		/*
		//define positions for running phases depending on hall sensor/position CW
		#define CW101  step2
		#define CW100  step1
		#define CW110  step6
		#define CW010  step5
		#define CW011  step4
		#define CW001  step3
		//define positions for running phases depending on hall sensor/position CCW
		#define CCW101  step1
		#define CCW100  step6
		#define CCW110  step5
		#define CCW010  step4
		#define CCW011  step3
		#define CCW001  step2
		*/
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
		switch (current_hall_status){
			case 0b00000101:	//	0b00000101	
				current_motor_position = 1;
				if(motor_direction == direction_CW && last_motor_position == 6){ //check previous motor position dependence on direction
					right_position = 1;
					} else if (motor_direction == direction_CCW && last_motor_position == 2){ //direction CCW
					right_position = 1;
					}
				last_motor_position=current_motor_position;
			break;
			case 0b00000100://0b00000100
				current_motor_position = 2;
				if(motor_direction == direction_CW && last_motor_position == 1){ //direction CW
					right_position = 1;
					} else if (motor_direction == direction_CCW && last_motor_position == 3){ //direction CCW
					right_position = 1;
					}
				last_motor_position=current_motor_position;
			break;
			case 0b00000110://0b00000110
				current_motor_position = 3;
				if(motor_direction == direction_CW && last_motor_position == 2){ //direction CW
					right_position = 1;
					} else if (motor_direction == direction_CCW && last_motor_position == 4){ //direction CCW
					right_position = 1;
					}
				last_motor_position=current_motor_position;
			break;
			case 0b00000010://0b00000010
				current_motor_position = 4;
				if(motor_direction == direction_CW && last_motor_position == 3){ //direction CW
					right_position = 1;
					} else if (motor_direction == direction_CCW && last_motor_position == 5){ //direction CCW
					right_position = 1;
					}
				last_motor_position=current_motor_position;
			break;
			case 0b00000011://0b00000011
				current_motor_position = 5;
				if(motor_direction == direction_CW && last_motor_position == 4){ //direction CW
					right_position = 1;
					} else if (motor_direction == direction_CCW && last_motor_position == 6){ //direction CCW
					right_position = 1;
					}
				last_motor_position=current_motor_position;
			break;
			case 0b00000001://0b00000001
				current_motor_position = 6;
				if(motor_direction == direction_CW && last_motor_position == 5){ //direction CW
					right_position = 1;
					} else if (motor_direction == direction_CCW && last_motor_position == 1){ //direction CCW
					right_position = 1;
					}
				last_motor_position=current_motor_position;
			break;
		   }//end of switch
		   
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by check_motor_position
void  wrong_spin_detector(){
	
	//wrong spin detector
	last_motor_position = current_motor_position; //update last state
	if(right_position == 1){ //motor position is OK
		output_controller = 1;
		Clear();
		Write_String("spin is OK");
		_delay_ms(show_delay);
	}
	else{
		wrong_spin=1;
		if(position_error == 0){
			if(motor_direction == direction_CW){
				Clear();
				Write_String("CW wrong_spin");
				_delay_ms(show_delay);
			}
			else if (motor_direction == direction_CCW){
				Clear();
				Write_String("CCW wrong_spin");
				_delay_ms(show_delay);
			}
			last_position_error = last_motor_position;
		}
	}
	
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by (TIMER1 over flow vector)
void  output_status(){
	
		if(output_controller){
				output_controller = 0;//reset  output controller
				if(startup_sequence){
					startup_sequence = 0; //reset  startup sequence						
					startup_commutation();	
					last_hall_status = 0; //reset last hall status
					}							
				else if(wrong_spin_status){
						spin_free; //wrong spin, emit break or spin
						wrong_spin_status = 0;
						}
		else{
			hall_status_output();//output depending on direction and position		
			}
		 }
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by output_status
void  startup_commutation(){
	
	uint16_t startup_DALAY[startup_commut + 1] = startup_delays; //startup delays
	//do a complete startup loop starting from the current motor position
	uint8_t i = 0;
	uint8_t go = current_motor_position;
	
	while( i < startup_commut) {//define startup_commut  6 number of startup commutations
		spin_free;
		_delay_us(15);
		//-------------------------------------------------------------------------------------------------------------		
		switch (go) {
			case 1:
			current_motor_position = 1; //update last motor position
			if(motor_direction == direction_CW) { //direction CW
				CW101;//#define CW101  step2 AH-CL
				} 
				else if (motor_direction == direction_CCW) { //direction CCW
				CCW101;//#define CCW101  step1  AH-BL
			    }
			break;
			case 2:
			current_motor_position = 2; //update last motor position
			if(motor_direction == direction_CW) { //direction CW
				CW100;//#define CW100  step1  AH-BL
				} 
				else if (motor_direction == direction_CCW) { //direction CCW
				CCW100;//#define CCW100  step6  CH-BL
			    }
			break;
			case 3:
			current_motor_position = 3; //update last motor position
			if(motor_direction == direction_CW) { //direction CW
				CW110;//#define CW110  step6  CH-BL
				} 
				else if (motor_direction == direction_CCW) { //direction CCW
				CCW110;//#define CCW110  step5 CH-AL
			    }
			break;
			case 4:
			current_motor_position = 4; //update last motor position
			if(motor_direction == direction_CW) { //direction CW
				CW010;//#define CW010  step5 CH-AL
				} 
				else if (motor_direction == direction_CCW) { //direction CCW
				CCW010;//#define CCW010  step4 BH-AL
			    }
			break;
			case 5:
			current_motor_position = 5; //update last motor position
			if(motor_direction == direction_CW) { //direction CW
				CW011;//#define CW011  step4 BH-AL
				} 
				else if (motor_direction == direction_CCW) { //direction CCW
				CCW011;//#define CCW011  step3 BH-CL
			    }
			break;
			case 6:
			current_motor_position = 6; //update last motor position
			if(motor_direction == direction_CW) { //direction CW
				CW001;//#define CW001  step3 BH-CL
				} 
				else if (motor_direction == direction_CCW) { //direction CCW
				CCW001;//#define CCW001  step2 AH-CL
			    }
			break;
		}//end of switch

		//check startup direction
		if(motor_direction == direction_CW) { //direction CW
			go++;
			} else if (motor_direction == direction_CCW) { //direction CCW
			go--;
		  }//end of if

		if(go > 6)
		go = go-6;
		else if(go < 1)
		go = 6;

		uint16_t m = 0;
		//do delay for startup
		for(m=0; m < startup_DALAY[i]; m++) {
			_delay_us(startup_delay_multiplier);
		   }//end of for

		i++;
	}//end of while
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by output_status
void  hall_status_output(){
	
			switch (current_hall_status){
					case 0b00000101://0b00000101
					if(motor_direction == direction_CW) { //direction CW
						CW101;//#define CW101  step2 AH-CL
					   }
					else if (motor_direction == direction_CCW) { //direction CCW
						CCW101;//#define CCW101  step1  AH-BL
					   }
					break;
					case 0b00000100://0b00000100
					if(motor_direction == direction_CW) { //direction CW
						CW100;//#define CW100  step1  AH-BL
						}
					else if (motor_direction == direction_CCW) { //direction CCW
						CCW100;//#define CCW100  step6  CH-BL
						}
					break;
					case 0b00000110://0b00000110
					if(motor_direction == direction_CW) { //direction CW
						CW110;//#define CW110  step6  CH-BL
					   }
					else if (motor_direction == direction_CCW) { //direction CCW
						CCW110;//#define CCW110  step5 CH-AL
					   }
					break;
					case 0b00000010://0b00000010
					if(motor_direction == direction_CW) { //direction CW
						CW010;//#define CW010  step5 CH-AL
						}
					else if (motor_direction == direction_CCW) { //direction CCW
						CCW010;//#define CCW010  step4 BH-AL
						}
					break;
					case 0b00000011://0b00000011
					if(motor_direction == direction_CW) { //direction CW
						CW011;//#define CW011  step4 BH-AL
						}
					else if (motor_direction == direction_CCW) { //direction CCW
						CCW011;//#define CCW011  step3 BH-CL
						}
					break;
					case 1://0b00000001
					if(motor_direction == direction_CW) { //direction CW
						CW001;//#define CW001  step3 BH-CL
						}
					else if (motor_direction == direction_CCW) { //direction CCW
						CCW001;//#define CW001  step2 AH-CL
						}
					break;
			}
	
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by (TIMER1 over flow vector)
void  speed_controller(){
	
	if(speed_counter < speed_value){
		speed_counter++; //increase speed counter
		Clear();
		Write_String("speed_cnt =");print_value(speed_counter);
		_delay_ms(show_delay);
	    }//end of if
	else{
		speed_counter = 0;
		if(motor_flag){ //running status motor is started?
			PWM_controller = 1; //this clock emit output to  emit output error if we are not spinning the right way
			
			//update current spin direction
			if(wrong_spin){
				if(motor_direction == direction_CW){
					current_spin_direction = spin_CCW;
				   }
				else if (motor_direction == direction_CCW){
					current_spin_direction = spin_CW;
				   }
				wrong_spin = 0; //reset wrong spin counter
				wrong_spin_status = 1; //output wrong status
			    }
			else {
				//update current spin direction
				if(motor_direction == direction_CW){
					current_spin_direction = spin_CW;
				   }//end of if
				else if (motor_direction == direction_CCW){
					current_spin_direction = spin_CCW;
				   }//end of else if
			    }//end of else
		     }//end of if
	      }//end of else
}
//-------------------------------------------------------------------------------------------------------------
//edited OK function call by bldc_init  and running
uint16_t  set_speed(){
	
		cli();

		uint16_t pot_speed = 0;
		uint16_t pot_speed_old = 0;
		uint16_t pot_speed_value;	

		pot_speed = ADC_read(POT_speed_chnnel);
		
		pot_speed = ((int32_t)(64-10)* pot_speed +(int32_t)10* pot_speed)>>6; //read filtered pot speed
		Clear();
		Write_String("pot_speed = ");print_value(pot_speed);
		_delay_ms(show_delay);
		if(pot_speed != pot_speed_old) {
			pot_speed_old = pot_speed;
			pot_speed_value = (long)(pot_speed - 0) * (long)(100 - 0) / (long)(1024 - 0) + 0;
			}		
		speed_value = (long)speed_MIN - ((long)(pot_speed_value - 0) * (long)(speed_MIN - 0) / (long)(100 - 0) + 0);
		sei();
		Clear();
		Write_String("speed = ");print_value(speed_value);
		_delay_ms(show_delay);
	return speed_value;
}
//-------------------------------------------------------------------------------------------------------------
//edited OK
//-------------------------------------------------------------------------------------------------------------
unsigned int  ADC_read(unsigned int  channel){

	unsigned char sample;
	ADC_value = ADC_init(channel);//analog to digital conversion
	for(sample=0;sample<16;sample++){
		ADC_value += ADC_init(channel);// read ADC samples
	}
	ADC_value >>= 4;// take average of the 16 samples 'adc_val /= 16'  0b 1111 1111 >> 4 = 0b 0000 1111
	return  ADC_value;
}
//-------------------------------------------------------------------------------------------------------------
unsigned int ADC_init(unsigned int  channel){
	
	ADMUX |= (channel & 0b0000111);//set ADC channel : channel must be 0 to 7  (ADC0....ADC7)
	ADMUX |= (1<<REFS1) | (1<<REFS0);//Internal 2.56V Voltage Reference with external capacitor at AREF pin
	//ADMUX |= (1<<REFS0);//AVCC reference voltage  with external capacitor at AREF pin
	
	ADCSRA |=(0 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);// prescalar: 8 => 1MHz/8 = 128kHz
	//ADCSRA |=(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);// prescalar: 128 => 1MHz/128 = 8kHz
	ADCSRA |=(1<<ADATE);
	ADCSRA|= (1<<ADEN);//ADC Conversion enable
	//ADCSRA|= (1<<ADFR);//ADC Free Running Select
	//ADCSRA|= (1<<ADIE);//ADC Interrupt Enable
	ADCSRA|= (1<<ADSC);//ADC Start Conversion
	while((ADCSRA & (1<<ADIF))==0);	// Monitor end of conversion interrupt
	_delay_us(10);
	
	return (ADC);// return results
}
//--------------------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------------------
