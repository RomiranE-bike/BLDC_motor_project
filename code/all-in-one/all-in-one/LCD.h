/*
 * LCD.h
 *
 * Created: 7/27/2024 10:03:37
 *  Author: me
 */ 

//----------------------------------------------------------------------------------------------------
#ifndef LCD_H_
#define LCD_H_

//----------------------------------------------------------------------------------------------------
/*
  Hex Code	  Command to LCD instruction Register

  0x01    Clear display screen
  0x02	  Return home
  0x03    Reset process from data sheet
  0x04	  Decrement cursor (shift cursor to left)
  0x06	  Increment cursor (shift cursor to right)
  0x05	  Shift display right
  0x07	  Shift display left
  0x08	  Display off, cursor off
  0x0A	  Display off, cursor on
  0x0C	  Display on, cursor off
  0x0E	  Display on, cursor blinking
  0x0F	  Display on, cursor blinking
  0x10	  Shift cursor position to left
  0x14	  Shift the cursor position to the right
  0x18	  Shift the entire display to the left
  0x1C	  Shift the entire display to the right
  0x80	  Force cursor to the beginning ( 1st line)
  0xC0	  Force cursor to the beginning ( 2nd line)
  0x38	  8-bit, 2 line, 5×7 Dots
  0x30    8-bit, 1 Line, 5x7 Dots
  0x28    4-bit, 2 Line, 5x7 Dots
  0x20    4-bit, 1 Line, 5x7 Dots

  0x80+ address*        Set DDRAM address or coursor position on display
  0x40+address**       Set CGRAM address or set pointer to CGRAM location
*/
//----------------------------------------------------------------------------------------------------
//#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "beep.h"
#include "io.h"
//----------------------------------------------------------------------------------------------------
//LCD pin out setting 
//1    2   3   4   5   6    7   8   9   10  11   12  13  14
//VSS VDD VEE  RS  RW  EN   D0  D1  D2  D3  D4   D5  D6  D7 
//             PD4 --  PD5                  PD0  PD1 PD2 PD3

#define data_Port        PORTD    //Define LCD data port
#define data_ddr         DDRD     //Define LCD data port direction		 
#define RS               PD4	  //Define Register Select pin
#define EN               PD5 	  //Define Enable signal pin
//----------------------------------------------------------------------------------------------------
// Convenience def for some of the most useful built-in HC44780(U) characters.
#define ALPHA       0xE0
#define BETA        0xE2
#define INFINITY    0xF3
#define MU          0xE4
#define OHM         0xF4
#define RHO         0xE6
#define SIGMA       0xF6
#define PI          0xF7
#define FOOTNOTE1   0xEB    // math complex conjugate symbol as well ?
#define FOOTNOTE2   0xB7
#define MEAN        0xF8    // mathematical mean (x_bar)
#define DEGREE      0xDF
#define THEREEXISTS 0xAE
#define BLOCK       0xFF    // a filled vertical rectangle
//----------------------------------------------------------------------------------------------------
void lcd_init();
void set_cmd(char cmd);
void Write_Char(char str);
void Write_String(char *str);
void Shift_Right();
void Shift_Left();
void Clear();
void Reset();
void Set_Cursor(char row, char calm);
void print_menu(const uint8_t *FlashLoc, uint8_t x, uint8_t y);
void print_value(volatile uint8_t value);
//----------------------------------------------------------------------------------------------------
void lcd_init(){ // function for initialize

        data_Port=0x00;
        _delay_ms(200);
        //Reset();
        set_cmd(0x02);   //to initialize LCD in 4-bit mode.
        set_cmd(0x02);
        set_cmd(0x08);
        set_cmd(0x00);
        set_cmd(0x0C);   // Disable Cursor
        set_cmd(0x00);
        set_cmd(0x06);   // Move Cursor
        //set_cmd(0x28); // to initialize LCD in 2 lines, 5X7 dots and 4bit mode.
        //set_cmd(0x83); // placing the cursor on the 4th position DDRAM address will be 0x03 and the command will be 0x80+0x03 = 0x83
        //set_cmd(0x01); // Clear LCD
        //set_cmd(0x80); // Move to Position Line 1, Position 1

}
//----------------------------------------------------------------------------------------------------
void set_cmd(char cmd){

        data_Port &= ~ (1<<RS);		// RS=0, command reg. 
        data_Port |= (1<<EN);		// Enable pulse 
        _delay_ms(10);
        data_Port = cmd | 0x20 ;         //data | 0x20
        _delay_ms(10);
        data_Port &= ~ (1<<EN);           //00000000  EN = 0
        _delay_ms(10);
}
//----------------------------------------------------------------------------------------------------
void Write_Char(char str){

        char temp0,temp1;                         //mask lower nibble because PD0-PD3 pins are used.
        temp0 = str & 0x0F;
        temp1 = str & 0xF0;
        //---------------------------------------------------------------------------
        data_Port |= (1<<RS);                                        //00010000  RS = 1
        data_Port |= (1<<EN);                                         //00100000  EN = 1
        _delay_ms(10);
        data_Port = (temp1>>4) | 0x30 ;               //shift right 4-bit and mask with RS and EN and data transfer
        _delay_ms(10);
        data_Port &= ~ (1<<EN);                                         //00100000  EN = 0
        _delay_ms(10);
        //---------------------------------------------------------------------------
        data_Port |= (1<<RS);                                         //00010000  RS = 1
        data_Port |= (1<<EN);                                         //00100000  EN = 1
        _delay_ms(10);
        data_Port = (temp0) | 0x30 ;                  //and mask with RS and EN and data transfer
        _delay_ms(10);
        data_Port &= ~ (1<<EN);                                         //00100000  EN = 0
        _delay_ms(10);
}
//----------------------------------------------------------------------------------------------------
void Write_String(char *str){                     //store address value of the string in pointer *message

        int i;
        for(i=0;str[i]!='\0';i++){                    // loop will go on till the NULL character in the string
            Write_Char(str[i]);                       // sending data on LCD byte by byte
        }
}
//----------------------------------------------------------------------------------------------------
void Shift_Right(){
       int i;
        for(i=0;i<15;i++){
            set_cmd(0x01);
            set_cmd(0x0C);
            }
}
//----------------------------------------------------------------------------------------------------
void Shift_Left(){
        int i;
        for(i=0;i<15;i++){
            set_cmd(0x01);
            set_cmd(0x08);
            }
}
//----------------------------------------------------------------------------------------------------
void Clear(){
        set_cmd(0x00);
        set_cmd(0x01);
}
//----------------------------------------------------------------------------------------------------
void Reset(){
        /************ Reset process from datasheet *********/
        set_cmd(0x03);
        _delay_ms(50);
        set_cmd(0x03);
        _delay_ms(110);
        set_cmd(0x03);
        /***************************************************/
   }
//----------------------------------------------------------------------------------------------------
void Set_Cursor(char row, char calm){
	char temp0,temp1,temp2;
	if(row == 1)
	{
		temp0 = 0x80 + calm;
		temp1 = temp0>>4;
		temp2 = (0x80+ calm) & 0x0F;
		set_cmd(temp1);
		set_cmd(temp2);
	}
	else if(row == 2)
	{
		temp0 = 0xC0 + calm;
		temp1 = temp0>>4;
		temp2 = (0xC0+ calm) & 0x0F;
		set_cmd(temp1);
		set_cmd(temp2);
	}
}
//----------------------------------------------------------------------------------------------------
void print_value(volatile uint8_t value){
	char printbuff[16];
	itoa(value, printbuff, 10);
	Write_String(printbuff);
}
//----------------------------------------------------------------------------------------------------
void print_menu(const uint8_t *FlashLoc, uint8_t x, uint8_t y){
	uint8_t i;
	Set_Cursor(x,y);
	for(i=0;(uint8_t)pgm_read_byte(&FlashLoc[i]);i++){
		Write_Char((uint8_t)pgm_read_byte(&FlashLoc[i]));
	}
}
//----------------------------------------------------------------------------------------------------
/*
//#define CURSOR_ON	 0		//   cursor on
//#define CURSOR_OFF	 1		//   cursor off
//#define BLINK_ON	 2		//   cursor blink ON
//#define BLINK_OFF	 3		//   cursor blink OFF

  0x08	  Display off, cursor off
  0x0A	  Display off, cursor on
  0x0C	  Display on, cursor off
  0x0E	  Display on, cursor blinking off
  0x0F	  Display on, cursor blinking on

void CursorMode(unsigned char mode)
{
	if (mode == CURSOR_ON)
	{
		set_cmd(0x0A);				//  blinking off
	}
    else if (mode == CURSOR_OFF)
	{
		set_cmd(0x0C);				// cursor off
	}
	else if (mode == BLINK_ON)
	{
		set_cmd(0x0F);				// blinking on
	}
    else if (mode == BLINK_OFF)
	{
		set_cmd(0x0E);				// blinking on
	}

}   */
//----------------------------------------------------------------------------------------------------
#endif /* LCD_H_ */
//----------------------------------------------------------------------------------------------------