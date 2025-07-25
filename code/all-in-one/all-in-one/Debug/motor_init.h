/*
 * motor_init.h
 *
 * Created: 8/3/2024 11:28:28
 *  Author: me
 */ 
//--------------------------------------------------------------------------------------------------------------
#ifndef MOTOR_INIT_H_
#define MOTOR_INIT_H_
//--------------------------------------------------------------------------------------------------------------
//Inputs/Outputs atmega16
/*
 * PWM in pin       PB3  //OC0
 * High  A          PD5  //OC1A
 * LOW   A          PC1
 * HIGH  B          PD4  //OC1B
 * LOW   B          PC3
 * HIGH  C          PD7  //OC2
 * LOW   C          PC5
 * Comparator       PB2  //AIN0
 * start            PB5
 * stop             PB4
*/
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include "io.h"
//--------------------------------------------------------------------------------------------------------------
#define     AH      PC0
#define     AL      PC1
#define     BH      PC2
#define     BL      PC3
#define     CH      PC4
#define     CL      PC5
//--------------------------------------------------------------------------------------------------------------
#define     AH_BL   PORTC=0b00001001;
#define     AH_CL   PORTC=0b00100001;
#define     BH_CL   PORTC=0b00100100;
#define     BH_AL   PORTC=0b00000110;
#define     CH_AL   PORTC=0b00010010;
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
#define on_off_button        PA4
#define startup_button       PA5
#define speed_button         PA6
#define direction_button     PA7

#define POT_speed_chnnel     3//PA3 OR ADC3

//--------------------------------------------------------------------------------------------------------------
#define d_commut      100
#define High          100
#define Low           0
#define mid           50
//--------------------------------------------------------------------------------------------------------------
#define SENSE_U     ADMUX = 0; // Back EMF input phase U
#define SENSE_V     ADMUX = 1; // Back EMF input phase V
#define SENSE_W     ADMUX = 2; // Back EMF input phase W
#define SENSE_UVW   (ACSR & (1 << ACO)) // Comparator output(PB2 AIN0)
//--------------------------------------------------------------------------------------------------------------
unsigned int             startup ;
unsigned int             step = 1;
unsigned char            speed;
unsigned int             i;
unsigned int             error_status=0;
unsigned int             spinning_flag = 0;
volatile unsigned char   rotor_run; // Back EMF pulse counter

//----------------------------------------------------------------------------------------------------------------
#define     PHASE_ALL_OFF  PORTC =0b00000000;//IR2103(pin 2 inH pin 3 inL)
#define     START_PWM 50 // Minimum PWM at startup
//----------------------------------------------------------------------------------------------------------------
unsigned char            position=0;
volatile unsigned char   sequence_step = 0;
volatile unsigned char   rotor_run = 0; // Back EMF pulse counter
//--------------------------------------------------------------------------------------------------------------
#endif /* MOTOR_INIT_H_ */
//--------------------------------------------------------------------------------------------------------------