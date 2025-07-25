/*
 * LCD.h
 *
 * Created: 6/7/2024 10:10:46
 *  Author: me
 * Program to interface LCD in 4 bit mode with AVR micro controller
 */ 
//--------------------------------------------------------------------------------------------------------------

#ifndef LCD_H_
#define LCD_H_
//--------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <util/delay.h>
#include "io.h"
//--------------------------------------------------------------------------------------------------------------
#define LCD_Dir  DDRD			 //Define LCD data port direction 
#define LCD_Port PORTD			 //Define LCD data port 
#define RS PD0				     //Define Register Select pin 
#define EN PD1 				     //Define Enable signal pin 
//--------------------------------------------------------------------------------------------------------------
#define    message_1 " BLDC v.0.1 "
#define    message_2 " running! "
#define    message_3 " stoped ! "
#define    message_4 " waiting! "
#define    show_delay  1
//--------------------------------------------------------------------------------------------------------------
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
//--------------------------------------------------------------------------------------------------------------
void lcd_init();
void set_cmd(char cmd);
void Write_Char(unsigned char data);
void Write_String(char *str);
void Shift_Right();
void Shift_Left();
void Clear();
void Reset();
void Write_String_Pos(char row, char pos, char *str);
void Set_Cursor(char row, char calm);
void print_value(volatile uint8_t value);
void re_writ(int row,int colum);
//--------------------------------------------------------------------------------------------------------------
void lcd_init(){ // function for initialize

        LCD_Dir = 0xFF;			// Make LCD port direction as o/p
        _delay_ms(20);			// LCD Power ON delay always >15ms		 
		
		set_cmd(0x02);		 //send for 4 bit initialization of LCD  
		set_cmd(0x28);              // 2 line, 5*7 matrix in 4-bit mode
		set_cmd(0x0c);              // Display on cursor off
		set_cmd(0x06);              // Increment cursor (shift cursor to right)
		set_cmd(0x01);              // Clear display screen
		_delay_ms(2);
}
//--------------------------------------------------------------------------------------------------------------
void set_cmd(char cmd){
	
	    LCD_Port  = (LCD_Port & 0x0F) | (cmd & 0xF0); /* sending upper nibble */
	    LCD_Port &= ~ (1<<RS);		/* RS=0, command reg. */
	    LCD_Port |= (1<<EN);		/* Enable pulse */
	    _delay_us(1);
	    LCD_Port &= ~ (1<<EN);
	    _delay_us(200);
	    LCD_Port = (LCD_Port & 0x0F) | (cmd << 4);  /* sending lower nibble */
	    LCD_Port |= (1<<EN);
	    _delay_us(1);
	    LCD_Port &= ~ (1<<EN);
	    _delay_ms(2);
}
//--------------------------------------------------------------------------------------------------------------
void Write_Char(unsigned char data){

        LCD_Port  = (LCD_Port & 0x0F) | (data & 0xF0); /* sending upper nibble */
        LCD_Port |= (1<<RS);		/* RS=1, data reg. */
        LCD_Port |= (1<<EN);
        _delay_us(1);
        LCD_Port &= ~ (1<<EN);
        _delay_us(200);
        LCD_Port = (LCD_Port & 0x0F) | (data << 4); /* sending lower nibble */
        LCD_Port |= (1<<EN);
        _delay_us(1);
        LCD_Port &= ~ (1<<EN);
        _delay_ms(2);
}
//--------------------------------------------------------------------------------------------------------------
void Write_String(char *str){     //store address value of the string in pointer *message
		
		while(*str > 0){
			Write_Char(*str++);// Write each char of string till the NULL  
		}		 
}
//--------------------------------------------------------------------------------------------------------------
void Shift_Right(){
       int i;
        for(i=0;i<15;i++){
	        set_cmd(0x1c);/* shift entire display right */
	        _delay_ms(300);
           }
}
//--------------------------------------------------------------------------------------------------------------
void Shift_Left(){
        int i;
        for(i=0;i<15;i++){
	        set_cmd(0x18);/* shift entire display right */
	        _delay_ms(300);
           }
}
//--------------------------------------------------------------------------------------------------------------
void Clear(){
        set_cmd (0x01);		/* Clear display */
        set_cmd (0x80);		/* Cursor at home position */
}
//--------------------------------------------------------------------------------------------------------------
void re_writ(int row,int colum){
	Set_Cursor(row,colum);
	Write_String("                ");
}
//--------------------------------------------------------------------------------------------------------------
void Reset(){
        /************ Reset process from datasheet *********/
        set_cmd(0x03);
        _delay_ms(5);
        set_cmd(0x03);
        _delay_ms(11);
        set_cmd(0x03);
        /***************************************************/
   }
//--------------------------------------------------------------------------------------------------------------
void Stop(){
	Clear();
	Write_String(message_1);                         //message_1 " BLDC v.0.1 "
	set_cmd(0xC0);		// Go to 2nd line
	Write_String(message_3);                         // message_3 " stop  ! "
	//delay_ms(50);             //100 ms
	Shift_Right();
	_delay_ms(50);
	Shift_Left();
	_delay_ms(50);
}
//--------------------------------------------------------------------------------------------------------------
void Write_String_Pos(char row, char pos, char *str){
	
	if (row == 1 && pos<16)
	set_cmd((pos & 0x0F)|0x80);	// Command of first row and required position<16 
	else if (row == 2 && pos<16)
	set_cmd((pos & 0x0F)|0xC0);	// Command of first row and required position<16 
	Write_String(str);
}
//--------------------------------------------------------------------------------------------------------------
void Set_Cursor(char row, char calm){

	if(row == 1){
		set_cmd(( calm & 0x0F) | 0x80);
	  }
	else if(row == 2){
		set_cmd((calm & 0x0F) | 0xC0 );
	  }
}
//--------------------------------------------------------------------------------------------------------------
void print_value(volatile uint8_t value){
	char printbuff[16];
	itoa(value, printbuff, 10);
	Write_String(printbuff);
}
//--------------------------------------------------------------------------------------------------------------
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

}   
//--------------------------------------------------------------------------------------------------------------
//dtostrf(V_phase, 5, 2, Vbuffer);
//dtostrf(U_phase, 5, 2, Ubuffer);
//dtostrf(W_phase, 5, 2, Wbuffer);

//unsigned char       str[16];
//sprintf(str,"U=%dV=%dW=%d",U,V,W); // use sprintf() to compose the string str
//Write_String(str);

//sprintf(buffer,"U=%dV=%dW=%d",U_phase,V_phase,W_phase); // use sprintf() to compose the string str
//Write_String(buffer);


char printbuff[16];
int debug_state=102;
itoa(debug_state, printbuff, 10);
Write_String(printbuff);
*/
//--------------------------------------------------------------------------------------------------------------

#endif //LCD_H_ //
//--------------------------------------------------------------------------------------------------------------
