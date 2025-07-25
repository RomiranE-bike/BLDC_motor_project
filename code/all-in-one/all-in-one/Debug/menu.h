/*
 * menu.h
 *
 * Created: 8/30/2024 14:41:48
 * edited : me
 */ 
//--------------------------------------------------------------------------------------------------------
#ifndef MENU_H_
#define MENU_H_
//--------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "beep.h"
#include "io.h"
#include "LCD.h"
#include "stepper.h"
#include "NTC.h"
#include "BLDC.h"
#include "timer.h"
//--------------------------------------------------------------------------------------------------------
//flag required for exiting from functions loops
//uint8_t Flag=1; //initial by io.h
//Function prototypes
//Initial menu
void menu_init(void);
//Functions for each menu item
void main_menu_message(void);
void shift_right(void);
void shift_left(void);
void blink_fast(void);
void blink_slow(void);
//--------------------------------------------------------------------------------------------------------
//SubMenu and Function table pointer update
uint8_t MFIndex(uint8_t, uint8_t);
//--------------------------------------------------------------------------------------------------------
typedef void (*FuncPtr)(void);
//function pointer
FuncPtr FPtr;
//--------------------------------------------------------------------------------------------------------
//Structure describes current menu and sub menu state
struct Menu_State{
	uint8_t menuNo;//1,2,3,4
	uint8_t subMenuNo;//1,2,3
}MN;
//--------------------------------------------------------------------------------------------------------
//Menu Strings in flash
//menu 1
const uint8_t Text1[] PROGMEM="    WELCOME    ";
//Sub menus of menu 1
const uint8_t Text2[] PROGMEM="All_in one v1.2";
//menu 2
const uint8_t Q_LINE[] PROGMEM="Q_LINE v3.1";
//Sub menus of menu 2
const uint8_t Up[]    PROGMEM="tools_up";
const uint8_t Down[]  PROGMEM="tools_down";
const uint8_t Right[] PROGMEM="turn_right >>";
const uint8_t Left[]  PROGMEM="<< turn_left";
const uint8_t Start[] PROGMEM="line_start";

//menu 3
const uint8_t BLDC[] PROGMEM="BLDC v2.1";
//Sub menus of menu 3
const uint8_t Motor_start[] PROGMEM="soft_start >> ";
const uint8_t Motor_run[] PROGMEM="motor_run >> ";
//menu 4
const uint8_t NTC[] PROGMEM="NTC_Sensor";
//Sub menus of menu 4
const uint8_t Thermo_action[] PROGMEM="thermo_action";

//menu 5
const uint8_t Shift[] PROGMEM="shift LED";
//sub menus of menu 5
const uint8_t ShiftRight[] PROGMEM="shift_Right >>";
const uint8_t ShiftLeft[] PROGMEM="<< shift_Left";
//menu 6
const uint8_t Blink[] PROGMEM="Blink LED";
//sub menus of menu 6
const uint8_t Fast[] PROGMEM="Fast";
const uint8_t Slow[] PROGMEM="Slow";

//more menus and sub menus can be added.
//--------------------------------------------------------------------------------------------------------
//Arrays of pointers to menu strings stored in flash
const uint8_t *MENU[] ={
	Text1,	//menu 1 string
	Q_LINE,	//menu 2 string
	BLDC,	//menu 3 string
	NTC,	//menu 4 string
	Shift,	//menu 5 string
	Blink	//menu 5 string
};
//--------------------------------------------------------------------------------------------------------
const uint8_t *SUBMENU[] ={
	Text2,//sub menus of menu 1 
	Up, Down, Right, Left, Start,//MN206,//MN107,	//sub menus of menu 2
	Motor_start, Motor_run,			//sub menus of menu 3
	Thermo_action, 			//sub menus of menu 4
	ShiftRight, ShiftLeft,			//sub menus of menu 5
	Fast, Slow			//sub menus of menu 6
};
//--------------------------------------------------------------------------------------------------------
//Menu structure
//[0] -Number of first level  menu items
//[1]...[n] number of second level menu items
//EG. MSTR2[1] shows that menu item 1 has 3 sub menus
const uint8_t MSTR2[] PROGMEM ={
	
	6,	//number of menu items
	1,	//Number of sub menu items of menu item 1
	5,	//Number of sub menu items of menu item 2
	2,	//Number of sub menu items of menu item 3
	1,	//Number of sub menu items of menu item 4
	2,	//Number of sub menu items of menu item 5
	2	//Number of sub menu items of menu item 6
};
//--------------------------------------------------------------------------------------------------------
//Array of function pointers in Flash
const FuncPtr FuncPtrTable[] PROGMEM={
	main_menu_message,	 	
	tools_up, tools_down,turn_Right,turn_Left,line_start,//Line_stop,	//functions for sub menus of menu 1
	soft_start,motor_run, 			//functions for sub menus of menu 2
	thermo_action,
	shift_right,shift_left,  			//functions for sub menus of menu 3
	blink_fast, blink_slow,			//functions for sub menus of menu 4
};
//--------------------------------------------------------------------------------------------------------
void menu_init(void){
	MN.menuNo=1;
	MN.subMenuNo=1;
	Clear();
	print_menu(MENU[(MN.menuNo-1)],1,1);
	print_menu(SUBMENU[(MN.subMenuNo-1)],2,1);
	FPtr=(FuncPtr)pgm_read_word(&FuncPtrTable[0]);
}
//--------------------------------------------------------------------------------------------------------
//Timer0 overflow interrupt service routine
ISR(TIMER0_OVF_vect){
	
	//---------------------------------------------------------------------
	//if button menu_next pressed
	if ((A_inpout_PIN &(1<<menu_next))==0){
		_delay_ms(20);
		beep();
		if (MN.menuNo < pgm_read_byte(&MSTR2[0])){
			MN.menuNo++;
			MN.subMenuNo=1;
		}
		else{
			MN.menuNo=1;
		}
		
		Clear();
		//Display menu item
		print_menu(MENU[MN.menuNo-1],1,1);
		//Display sub menu item
		print_menu(SUBMENU[MFIndex(MN.menuNo, MN.subMenuNo)],2,1);
		//Assign function to function pointer
		FPtr=(FuncPtr)pgm_read_word(&FuncPtrTable[MFIndex(MN.menuNo, MN.subMenuNo)]);
		//set Flag to 0 means menu have changed
		Flag=0;
		//wait for button release
		while((A_inpout_PIN &(1<<menu_next))==0){}
	}
	//---------------------------------------------------------------------
	
	//if Button menu_back pressed
	if ((A_inpout_PIN &(1<<menu_back))==0){
		_delay_ms(20);
		beep();
		if (MN.menuNo==1){
			MN.menuNo=pgm_read_byte(&MSTR2[0]);
			MN.subMenuNo=1;
		}
		else{
			MN.menuNo--;
		}
		Clear();
		print_menu(MENU[MN.menuNo-1],1,1);
		print_menu(SUBMENU[MFIndex(MN.menuNo, MN.subMenuNo)],2,1);
		//Assign function to function pointer
		FPtr=(FuncPtr)pgm_read_word(&FuncPtrTable[MFIndex(MN.menuNo, MN.subMenuNo)]);
		//set Flag to 0 means menu have changed
		Flag=0;
		while((A_inpout_PIN &(1<<menu_back))==0){}
	}
	//---------------------------------------------------------------------
	//If Button sub_next pressed
	if ((A_inpout_PIN &(1<<sub_next))==0){
		_delay_ms(20);
		beep();
		if (MN.subMenuNo<pgm_read_byte(&MSTR2[MN.menuNo])){
			MN.subMenuNo++;
		}
		else{
			MN.subMenuNo=1;
		}
		Clear();
		print_menu(MENU[MN.menuNo-1],1,1);
		print_menu(SUBMENU[MFIndex(MN.menuNo, MN.subMenuNo)],2,1);
		//Assign function to function pointer
		FPtr=(FuncPtr)pgm_read_word(&FuncPtrTable[MFIndex(MN.menuNo, MN.subMenuNo)]);
		//set Flag to 0 means menu have changed
		Flag=0;
		while((A_inpout_PIN &(1<<sub_next))==0){}
	}
	//---------------------------------------------------------------------
	//If button sub_back pressed
	if ((A_inpout_PIN &(1<<sub_back))==0){
		_delay_ms(20);
		beep();
		if (MN.subMenuNo==1){
			MN.subMenuNo=pgm_read_byte(&MSTR2[MN.menuNo]);
		}
		else{
			MN.subMenuNo--;
		}
		Clear();
		print_menu(MENU[MN.menuNo-1],1,1);
		print_menu(SUBMENU[MFIndex(MN.menuNo, MN.subMenuNo)],2,1);
		//Assign function to function pointer
		FPtr=(FuncPtr)pgm_read_word(&FuncPtrTable[MFIndex(MN.menuNo, MN.subMenuNo)]);
		//set Flag to 0 means menu have changed
		Flag=0;
		while((A_inpout_PIN &(1<<sub_back))==0){}
	}

}
//--------------------------------------------------------------------------------------------------------
uint8_t MFIndex(uint8_t mn, uint8_t sb){
	uint8_t p=0;//points to menu in table of function pointer
	for(uint8_t i=0; i<(mn-1); i++){
		p=p+pgm_read_byte(&MSTR2[i+1]);
	}
	p=p+sb-1;
	return p;
}
//--------------------------------------------------------------------------------------------------------
void main_menu_message(void){
	//Clear();
	//print_menu("WELCOME",1,1);
	//print_menu("All_in one v1.2",2,1);
}
//--------------------------------------------------------------------------------------------------------
void shift_right(void){
	while(Flag){
		for(int i =0;i<8;i++){
		out_port = 0b10000000 >> i;
		_delay_ms(d_blink);				
		}
	}
}
//--------------------------------------------------------------------------------------------------------
void shift_left(void){
	while(Flag){
		for(int i=0;i<8;i++){
			out_port  = 0b00000001 << i;
			_delay_ms(d_blink);
		}		
	}
}
//--------------------------------------------------------------------------------------------------------
void blink_fast(void){
	while(Flag){
		out_port =0xff;// all Led s ON
		_delay_ms(d_blink);
		out_port =0x00;//all Led s OFF
		_delay_ms(d_blink);
	}
}
//--------------------------------------------------------------------------------------------------------
void blink_slow(void){
	while(Flag){
		out_port =0xff;//all Led s ON
		_delay_ms(d_blink);
		_delay_ms(d_blink);
		out_port =0x00;//all Led s OFF
		_delay_ms(d_blink);
		_delay_ms(d_blink);
	}
}
//-------------------------------------------------------------------------------------------------------------
#endif // MENU_H_
//--------------------------------------------------------------------------------------------------------------
