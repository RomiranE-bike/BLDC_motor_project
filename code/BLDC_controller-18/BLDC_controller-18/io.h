/*
 * io.h
 *
 * Created: 6/1/2024 15:19:37
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
//Inputs/Outputs atmega16
/*
 * PWM in pin       PB3  //OC0
 * High  A          PD5  //OC1A
 * LOW   A          PC2
 * HIGH  B          PD4  //OC1B
 * LOW   B          PC3
 * HIGH  C          PD7  //OC2
 * LOW   C          PC4
 * Comparator       PB2  //AIN0
 * start         PA3
 * stop          PA4
*/
//--------------------------------------------------------------------------------------------------------------
#ifndef IO_H_
#define IO_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
//--------------------------------------------------------------------------------------------------------------
#define     AH      PC7
#define     AL      PC6
#define     BH      PC5
#define     BL      PC4
#define     CH      PC3
#define     CL      PC2
//--------------------------------------------------------------------------------------------------------------
#define     AH_BL   PORTC=0b10010000;
#define     AH_CL   PORTC=0b10000100;
#define     BH_CL   PORTC=0b00100100;
#define     BH_AL   PORTC=0b01100000;
#define     CH_AL   PORTC=0b01001000;
#define     CH_BL   PORTC=0b00011000; 
//--------------------------------------------------------------------------------------------------------------
#define step1  AH_BL
#define step2  AH_CL
#define step3  BH_CL
#define step4  BH_AL
#define step5  CH_AL
#define step6  CH_BL
/*
#define step1  AH_BL 
#define step2  CH_AL 
#define step3  CH_BL 
#define step4  AH_CL 
#define step5  BH_CL 
#define step6  BH_AL 

#define step1  AH_CL
#define step2  AH_BL
#define step3  CH_BL
#define step4  CH_AL
#define step5  BH_AL
#define step6  BH_CL 

#define step1  CH_BL 
#define step2  CH_AL 
#define step3  BH_AL 
#define step4  BH_CL 
#define step5  AH_CL 
#define step6  AH_BL 

#define step1  AH_BL
#define step2  AH_CL
#define step3  BH_CL
#define step4  BH_AL
#define step5  CH_AL
#define step6  CH_BL
*/
//--------------------------------------------------------------------------------------------------------------
#define     Hall_DDR       DDRA
#define     Hall_PORT      PORTA
#define     Hall_PIN       PINA
#define     H1             PA0
#define     H2             PA1
#define     H3             PA2
//--------------------------------------------------------------------------------------------------------------
unsigned char       H1_status;
unsigned char       H2_status;
unsigned char       H3_status;
//--------------------------------------------------------------------------------------------------------------
unsigned char       comparator_status;
unsigned char       A_status;
unsigned char       B_status;
unsigned char       C_status;
//--------------------------------------------------------------------------------------------------------------
#define button_DDR          DDRA
#define button_PORT         PORTA
#define button_PIN          PINA

#define on_off_button        PA3
#define startup_button       PA4
#define speed_button       PA5
#define direction_button     PA6

#define POT_speed_chnnel     7//PA7 OR ADC7
#define G_LED_ON        PORTC |= (1<<PC0);
#define G_LED_OFF       PORTC &= ~(1<<PC0);
#define R_LED_ON        PORTC |= (1<<PC1);
#define R_LED_OFF       PORTC &= ~(1<<PC1);
//--------------------------------------------------------------------------------------------------------------
#define d_commut    100
#define H           100
#define L           0
#define X           50
//--------------------------------------------------------------------------------------------------------------
#define SENSE_U     ADMUX = 2; // Back EMF input phase U
#define SENSE_V     ADMUX = 3; // Back EMF input phase V
#define SENSE_W     ADMUX = 4; // Back EMF input phase W
#define SENSE_UVW   (ACSR & (1 << ACO)) // Comparator output
//--------------------------------------------------------------------------------------------------------------
unsigned int             startup ;
unsigned int             step = 1;
unsigned int             start_flag = 0;
unsigned char            speed;
unsigned int             i;
//unsigned int             sequence_step ;
unsigned int             error_status=0;
unsigned int             running_flag = 0;
unsigned int             start = 0;
volatile unsigned char   rotor_run; // Back EMF pulse counter
//volatile unsigned int    commut = 0;

//--------------------------------------------------------------------------------------------------------------
void io_init();
//--------------------------------------------------------------------------------------------------------------
void io_init(){
	// I/O ports
	DDRA   = 0x00;//set input
	PORTA  = 0xff;//set pull up resistor
	
	DDRB  &= ~((1 << PB3)|(1 << PB2));//set input for BEMF comparator signal
	PORTB |=  (1 << PB3)|(1 << PB2);//set pull up resistor
	
	DDRB  |=   (1 << PB7)|(1 << PB6)|(1 << PB5)|(1 << PB4)|(1 << PB1)|(1 << PB0);//set output
	PORTB &= ~((1 << PB7)|(1 << PB6)|(1 << PB5)|(1 << PB4)|(1 << PB3)|(1 << PB1)|(1 << PB0));//set low
	
	DDRC  = 0xff;//set output
	PORTC = 0x00;//set low
	
	DDRD  = 0xff;//set output
	PORTD = 0x00;//set low
	
}
//--------------------------------------------------------------------------------------------------------------
#endif //IO_H_ 
//--------------------------------------------------------------------------------------------------------------