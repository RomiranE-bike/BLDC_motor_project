/*
 * adc.h
 * ver.2.0
 * Created: 1/30/2024 17:03:29
 *  edited: me
 */ 

//-------------------------------------------------------------------------------------------------------------
#ifndef ADC_H_
#define ADC_H_
//-------------------------------------------------------------------------------------------------------------
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD.h"
//-------------------------------------------------------------------------------------------------------------

unsigned int  ADC_value;

float        U_phase=0;
char         Ubuffer[7];
float        V_phase=0;
char         Vbuffer[7];
float        W_phase=0;
char         Wbuffer[7];
unsigned char       buffer[16];
//-------------------------------------------------------------------------------------------------------------
unsigned int  ADC_init(unsigned int  channel);
unsigned int  ADC_read(unsigned int  channel);
void ADC_multi_read(void);
void print_volt(unsigned int data1,unsigned int data2,unsigned int data3);


void adc_setchannel(uint8_t channel);
uint16_t adc_readsel();
uint16_t adc_read(uint8_t channel);
void adc_init();
//-------------------------------------------------------------------------------------------------------------
unsigned int  extra_button_value;
unsigned int  extera_button_read(unsigned int channel);
//-------------------------------------------------------------------------------------------------------------
// ADC sample at specified channel, return 10-bit result
unsigned int  ADC_init(unsigned int  channel){
	    
		
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
void ADC_multi_read(void){	
	
   			unsigned int  sens_value[3];
								                       
			for(int i=0;i<3;i++){//set ADC channel : channel must be 0 to 7  (ADC0....ADC7)
					sens_value[i]=0;
					_delay_us(10);											
					sens_value[i] = ADC_read(i);//* 4.6099925  convert volt to m_volt
					_delay_us(10);
					ADMUX=0x00;								
					}
			U_phase  = (sens_value[0]);			
			V_phase  = (sens_value[1]);
			W_phase  = (sens_value[2]);
			
			
		     //button_value  =  (sens_value[5])/10;//button {22,50,68,84} for sensing ladder resistor.
			 //ladder_SW_button = button_value * 4.6099925;																											   															   												 
}
//-------------------------------------------------------------------------------------------------------------
void print_volt(unsigned int data1,unsigned int data2,unsigned int data3){
		
    Clear(); 	
	U_phase =  data1; //data1/204.8*18;//(data/18.618);
	V_phase =  data2;
	W_phase =  data3;
	dtostrf(U_phase, 4, 0, Ubuffer);
	dtostrf(V_phase, 4, 0, Vbuffer);
	dtostrf(W_phase, 4, 0, Wbuffer);
	Write_String("");
	Write_String(Ubuffer);
	Write_String("V");
	Write_String(Vbuffer);
	Write_String("V");
	Write_String(Wbuffer);
	Write_String("V");
}
//------------------------------------------------------------------------------------------------------------- 

//-------------------------------------------------------------------------------------------------------------
#endif /* ADC_H_ */
//-------------------------------------------------------------------------------------------------------------
/*
char buff[7];
unsigned int temp,R1,R2;
R1= 100000;R2=10000;
float volt =0;
temp=ADC_read(0);

//volt=temp/51.2;
//volt = (temp * 5.0) / 1024.0;
volt = temp/(1024/(5*100));
//volt =  (R2/(R1+R2)*volt);

dtostrf(volt, 5, 1, buff);
Write_String(buff);
*/


/*
 Note: This voltmeter is only as accurate as your reference voltage.
 * If you want four digits of accuracy, need to measure your AVCC well.
 * Measure either AVCC of the voltage on AREF and enter it here.

//#define REF_VCC 5.053
                               // measured division by voltage divider 
//#define VOLTAGE_DIV_FACTOR  3.114
//voltage = oversample16x() * VOLTAGE_DIV_FACTOR * REF_VCC / 4096;

uint16_t oversample16x(void) {
	uint16_t oversampledValue = 0;
	uint8_t i;
	for (i = 0; i < 16; i++) {
		sleep_mode();                    //chip to sleep, takes ADC sample 
		oversampledValue += ADC;                        // add them up 16x 
	}
	return (oversampledValue >> 2);         // divide back down by four 
}


*/

/*
void phase_status(){
	
}
//--------------------------------------------------------------------------------------------------------------
int phase_status_in(){
	
}
//--------------------------------------------------------------------------------------------------------------
//ADC phase conversion
//--------------------------------------------------------------------------------------------------------------
// Setting up the ADC
//--------------------------------------------------------------------------------------------------------------
//ADC speed conversion
//--------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Read higher 2 bits and Multiply with weight
/*
//-------------------------------------------------------------------------------------------------------------
//int Ain,AinLow;
// AinLow = (int)ADCL;		// Read lower byte
// Ain = (int)ADCH*256;		// Read higher 2 bits and Multiply with weight
//Ain = Ain + AinLow;
//return(Ain);			// Return digital value
*/
//-------------------------------------------------------------------------------------------------------------
//extera_button_read
/*
uint16_t extera_button_read(uint16_t channel){

		unsigned char sample;
		extra_button_value = adc_read(channel);//analog to digital conversion
		for(sample=0;sample<16;sample++){
			extra_button_value += adc_read(channel);// read ADC samples
		}
		extra_button_value >>= 4;  // take average of the 16 samples 'adc_val /= 16'  0b 1111 1111 >> 4 = 0b 0000 1111
		extra_button_value = (extra_button_value *5 + 512)/1024;
		return  extra_button_value;
		
		
		temp=temp/8; // >>3
		temp_c=(float)(temp)*.25;
		temp_f=temp_c*(9/5)+32;
	
}
*/
//-------------------------------------------------------------------------------------------------------------
//ADC Control and Status//page 205
/*
ADC Control and Status
Register A – ADCSRA
Bit 7 6 5 4 3 2 1 0
ADEN ADSC ADFR ADIF ADIE ADPS2 ADPS1 ADPS0 ADCSRA
Read/Write R/W R/W R/W R/W R/W R/W R/W R/W
Initial Value 0 0 0 0 0 0 0 0

Bit 7 – ADEN: ADC Enable
Bit 6 – ADSC: ADC Start Conversion
Bit 5 – ADFR: ADC Free Running Select
Bit 4 – ADIF: ADC Interrupt Flag
Bit 3 – ADIE: ADC Interrupt Enable
Bits 2:0 – ADPS2:0: ADC Prescaler Select Bits


Table 76. ADC Prescaler Selections
ADPS2 ADPS1 ADPS0 Division Factor
0 0 0 2
0 0 1 2
0 1 0 4
0 1 1 8
1 0 0 16
1 0 1 32
1 1 0 64
1 1 1 128

The ADC Data Register – ADCL and ADCH

ADLAR = 0
Bit 15 14 13 12 11 10 9 8
– – – – – – ADC9 ADC8 ADCH
ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADC1 ADC0 ADCL
7 6 5 4 3 2 1 0
Read/Write R R R R R R R R
R R R R R R R R
Initial Value 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0
Bit 15 14 13 12 11 10 9 8

ADLAR = 1
ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADCH
ADC1 ADC0 – – – – – – ADCL
7 6 5 4 3 2 1 0
Read/Write R R R R R R R R
R R R R R R R R
Initial Value 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

ADC9:0: ADC Conversion result


//-----------------------------------------------------------------------------------------------------
		//ADMUX &= ~(1 << ADLAR);// Right adjust ADC result for 10 bit reading (default)
		// -    -    -    -    -    -   ADC9 ADC8    ADCH
		//ADC7 ADC6 ADC5 ADC4 ADC3 ADC2 ADC1 ADC0    ADCL
		//ADMUX |=  (1 << ADLAR);// Left adjust ADC result to allow easy 8 bit reading
		//ADC9 ADC8 ADC7 ADC6 ADC5 ADC4 ADC3 ADC2   ADCH
		//ADC1 ADC0  -    -    -    -    -    -     ADCL
		----------------------------------------------------------------------------------		
*/
//-------------------------------------------------------------------------------------------------------------