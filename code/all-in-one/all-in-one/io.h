/*
 * io.h
 *
 * Created: 6/1/2024 15:19:37
 *  editor: me
 */ 
//--------------------------------------------------------------------------------------------------------------
//Inputs/Outputs atmega16
//--------------------------------------------------------------------------------------------------------------
#ifndef IO_H_
#define IO_H_
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
//---------------------------------------------------------------------------------------------------
#define    menu_back               PA5                // input with pull-up // press the U button
#define    menu_next               PA6                // input with pull-up // press the D button
#define    sub_back                PA4                // input with pull-up // press the L button
#define    sub_next                PA7                // input with pull-up // press the R button


#define    A_inpout_DDR           DDRA
#define    A_inpout_PORT          PORTA
#define    A_inpout_PIN           PINA

#define    Enter_start           PB4                // input with pull-up // press the START button
#define    Exit_stop             PB5                // input with pull-up // short the NOP switch
#define    Start_Limit           PB6                // input with pull-up // short the Start Limit switch
#define    End_Limit             PB7                // input with pull-up // short the End Limit switch

#define    B_inpout_DDR           DDRB
#define    B_inpout_PORT          PORTB
#define    B_inpout_PIN           PINB

#define    out_port              PORTC
#define    out_ddr               DDRC

//---------------------------------------------------------------------------------------------------
#define    action_port           PORTD
#define    action_ddr            DDRD
#define    LED                   PD6        // LED
#define    BUZ                   PD7        // BUZZER
#define    LED_ON                action_port |= 1 << LED;
#define    LED_off               action_port &= ~(1 << LED);
//--------------------------------------------------------------------------------------------------------------
//flag required for exiting from functions loops
uint8_t Flag=1;
uint8_t start_flag=0;
//--------------------------------------------------------------------------------------------------------------
void io_init();
//--------------------------------------------------------------------------------------------------------------
// io_init routine
void io_init(){

	DDRA=0x00;       //set all pins of port A as input
	PORTA=0xff;      //set all pins of port A pull-ups

	DDRB=0x00;      //make all pins of port B as input
	PORTB=0xff;     //enable all pins of port B pull-ups

	DDRC =0xff;     //set all pins of port C as output
	PORTC =0x00;    //set all pins of port C LOW

	DDRD =0xff;     //set all pins of port D as output
	PORTD =0x00;    //set all pins of port D LOW
}
//--------------------------------------------------------------------------------------------------------------
#endif //IO_H_ 
//--------------------------------------------------------------------------------------------------------------