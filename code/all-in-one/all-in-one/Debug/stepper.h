/*
 * stepper.h
 *
 * Created: 7/27/2024 10:14:00
 *  Author: me
 */ 
//------------------------------------------------------------------------------------------------------
#ifndef STEPPER_H_
#define STEPPER_H_
//------------------------------------------------------------------------------------------------------
//#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "LCD.h"
#include "io.h"
#include "beep.h"
//---------------------------------------------------------------------------------------------------
#define    message_1 " Q_Line   v3.1 "
#define    message_2 "Start running!"
#define    message_3 " Line stoped !"
#define    message_4 " Line waiting!"
//---------------------------------------------------------------------------------------------------
#define        d_out    100     // subject ready to test delay  ms
#define        d_in     100     // subject input delay  ms
#define        d_line   10       // line stepper delay :10 ms if clock set to 8MHZ /1 ms if clock set to 1MHZ
#define        d_tools  10       // tools stepper delay :10 ms if clock set to 8MHZ /1 ms if clock set to 1MHZ

#define        tools_port    PORTC
#define        line_port     PORTC

#define        line_step_1          line_port=0x10;  //step 1   0001  0000
#define        line_step_2          line_port=0x30;  //step 2   0011  0000
#define        line_step_3          line_port=0x20;  //step 3   0010  0000
#define        line_step_4          line_port=0x60;  //step 4   0110  0000
#define        line_step_5          line_port=0x40;  //step 5   0100  0000
#define        line_step_6          line_port=0xc0;  //step 6   1100  0000
#define        line_step_7          line_port=0x80;  //step 7   1000  0000
#define        line_step_8          line_port=0x90;  //step 8   1001  0000

#define        tools_step_1          tools_port=0x01;  //step 1  0000 0001
#define        tools_step_2          tools_port=0x03;  //step 2  0000 0011
#define        tools_step_3          tools_port=0x02;  //step 3  0000 0010
#define        tools_step_4          tools_port=0x06;  //step 4  0000 0110
#define        tools_step_5          tools_port=0x04;  //step 5  0000 0100
#define        tools_step_6          tools_port=0x0c;  //step 6  0000 1100
#define        tools_step_7          tools_port=0x08;  //step 7  0000 1000
#define        tools_step_8          tools_port=0x09;  //step 8  0000 1001
//---------------------------------------------------------------------------------------------------
void set_stepper(void);

void tools_up(void);
void tools_down(void);

void turn_Right(void);
void turn_Left(void);

void line_forward(void);
void line_reverse(void);

void line_start(void);
void Line_stop(void);

void test_Up(void);
void test_Down(void);
//---------------------------------------------------------------------------------------------------
// turn_Up routine
void tools_up(void){         // Rotate 4 upper bit of PORTA Motor clockwise with Half step sequence
	
	while(((B_inpout_PIN &(1<<Enter_start))==0)){   //sw4  // defualt delay ==1    //1111 0111
		tools_step_8;
		_delay_ms(d_tools);
		tools_step_7;
		_delay_ms(d_tools);
		tools_step_6;
		_delay_ms(d_tools);
		tools_step_5;
		_delay_ms(d_tools);
		tools_step_4;
		_delay_ms(d_tools);
		tools_step_3;
		_delay_ms(d_tools);
		tools_step_2;
		_delay_ms(d_tools);
		tools_step_1;
		_delay_ms(d_tools);
	}
}
//---------------------------------------------------------------------------------------------------
// turn_Down routine
void tools_down(void){     // Rotate 4 upper bit of PORTA Motor clockwise with Half step sequence    //main stepper turn forward
	
	
	while(((B_inpout_PIN &(1<<Enter_start))==0)){
		tools_step_1;
		_delay_ms(d_tools);
		tools_step_2;
		_delay_ms(d_tools);
		tools_step_3;
		_delay_ms(d_tools);
		tools_step_4;
		_delay_ms(d_tools);
		tools_step_5;
		_delay_ms(d_tools);
		tools_step_6;
		_delay_ms(d_tools);
		tools_step_7;
		_delay_ms(d_tools);
		tools_step_8;
		_delay_ms(d_tools);
	}

}
//---------------------------------------------------------------------------------------------------
// turn_Right routine
 void turn_Right(void){            // Rotate 4 low byte of PORTA Motor clockwise with Half step sequence

                  while( ((B_inpout_PIN &(1<<Enter_start))==0) && (B_inpout_PIN &(1<<Start_Limit))){     // defualt delay ==10     //1111 1101

                            line_step_1;
                            _delay_ms(d_line);
                            line_step_2;
                            _delay_ms(d_line);
                            line_step_3
                            _delay_ms(d_line);
                            line_step_4;
                            _delay_ms(d_line);
                            line_step_5;
                            _delay_ms(d_line);
                            line_step_6;
                            _delay_ms(d_line);
                            line_step_7;
                            _delay_ms(d_line);
                            line_step_8;
                            _delay_ms(d_line);
                            }
   }
//---------------------------------------------------------------------------------------------------
// turn_Left routine
 void turn_Left(void){               // Rotate 4 low byte of PORTA Motor anticlockwise with Half step sequence
     
                  while( ((B_inpout_PIN &(1<<Enter_start))==0) && (B_inpout_PIN &(1<<End_Limit))){     // defualt delay ==10

						line_step_8;
						_delay_ms(d_line);
						line_step_7;
						_delay_ms(d_line);
						line_step_6;
						_delay_ms(d_line);
						line_step_5;
						_delay_ms(d_line);
						line_step_4;
						_delay_ms(d_line);
						line_step_3;
						_delay_ms(d_line);
						line_step_2;
						_delay_ms(d_line);
						line_step_1;
						_delay_ms(d_line);
                    }
   }
//---------------------------------------------------------------------------------------------------
// test_Up routine
void test_Up(void){        // Rotate 4 upper bit of PORTA Motor clockwise with Half step sequence
        int cycle;
        beep();
        Clear();
        Set_Cursor(2,1);
        Write_String("test_Up");
        for(cycle=0;cycle<100;cycle++){   //sw4  // defualt delay ==1    //1111 0111
	        tools_step_8;
	        _delay_ms(d_tools);
	        tools_step_7;
	        _delay_ms(d_tools);
	        tools_step_6;
	        _delay_ms(d_tools);
	        tools_step_5;
	        _delay_ms(d_tools);
	        tools_step_4;
	        _delay_ms(d_tools);
	        tools_step_3;
	        _delay_ms(d_tools);
	        tools_step_2;
	        _delay_ms(d_tools);
	        tools_step_1;
	        _delay_ms(d_tools);
        }
            
    }
//---------------------------------------------------------------------------------------------------
// test_Down routine
void test_Down(void){     // Rotate 4 upper bit of PORTA Motor clockwise with Half step sequence    //main stepper turn forward
        int cycle;
        beep();
        Clear();
        Set_Cursor(2,1);
        Write_String("test_Down");
        for(cycle=0;cycle<100;cycle++){
	        tools_step_1;
	        _delay_ms(d_tools);
	        tools_step_2;
	        _delay_ms(d_tools);
	        tools_step_3;
	        _delay_ms(d_tools);
	        tools_step_4;
	        _delay_ms(d_tools);
	        tools_step_5;
	        _delay_ms(d_tools);
	        tools_step_6;
	        _delay_ms(d_tools);
	        tools_step_7;
	        _delay_ms(d_tools);
	        tools_step_8;
	        _delay_ms(d_tools);
        }
            
   }
//---------------------------------------------------------------------------------------------------
// Start routine
void line_start(void){
                    
          while(Flag  && (B_inpout_PIN &(1<<Exit_stop))){
			  
                     if(B_inpout_PIN &(1<<Start_Limit)){
	                    LED_ON;
					    Set_Cursor(1,1);
					    Write_String(" Q_Line   v3.1 ");
                        Set_Cursor(2,1);
                        Write_String("Forward  >>>>");
						
                        while(B_inpout_PIN &(1<<Start_Limit)){
							 line_forward();
							 if((B_inpout_PIN &(1<<Exit_stop))==0)       // breaking condition
							 break;
                             }//end of while

					    if((B_inpout_PIN &(1<<Start_Limit))==0){
							
							     LED_off;
                                 Line_stop();
                                 _delay_ms(d_out);
                                 test_Up();
                                 
								 Set_Cursor(1,1);
								 Write_String(" Q_Line   v3.1 ");
                                 Set_Cursor(2,1);
                                 Write_String("<<<<  Reverse");
								 
                                 while(B_inpout_PIN &(1<<End_Limit)){									  
									  line_reverse();
									  if((B_inpout_PIN &(1<<Exit_stop))==0)       // breaking condition
									  break;                             
                                     }//end of while

                                 if((B_inpout_PIN &(1<<End_Limit))==0){
                                        Line_stop();
                                        _delay_ms(d_in);
                                        test_Down();
                                       }//end of if
                        }//end of if
					 }//end of if

                     else{
                     Line_stop();
                      }//end of else
            }//end of while
			Line_stop();
 }
//---------------------------------------------------------------------------------------------------
// Forward routine
void line_forward(void){
            // Rotate 4 low byte of PORTA Motor clockwise with Half step sequence
			line_step_1;
			_delay_ms(d_line);
			line_step_2;
			_delay_ms(d_line);
			line_step_3;
			_delay_ms(d_line);
			line_step_4;
			_delay_ms(d_line);
			line_step_5;
			_delay_ms(d_line);
			line_step_6;
			_delay_ms(d_line);
			line_step_7;
			_delay_ms(d_line);
			line_step_8;
			_delay_ms(d_line);
                            

   }
//---------------------------------------------------------------------------------------------------
// Reverse routine
void line_reverse(void){
            // Rotate 4 low byte of PORTA Motor anticlockwise with Half step sequence
            line_step_8;
            _delay_ms(d_line);
            line_step_7;
            _delay_ms(d_line);
            line_step_6;
            _delay_ms(d_line);
            line_step_5;
            _delay_ms(d_line);
            line_step_4;
            _delay_ms(d_line);
            line_step_3;
            _delay_ms(d_line);
            line_step_2;
            _delay_ms(d_line);
            line_step_1;
            _delay_ms(d_line);
                        

    }
//---------------------------------------------------------------------------------------------------
// stop routine
void Line_stop(void){
	
			out_port=0x00;
			blink();       
 
   }
//---------------------------------------------------------------------------------------------------
#endif /* STEPPER_H_ */
//------------------------------------------------------------------------------------------------------