/*
 * display_functions.c
 *
 * Created: 03.05.2021 15:41:41
 *  Author: Ole H.
 */ 

#include "display_functions.h"
#include "display_data.h"

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

extern uint8_t dsp_data [4][4][20]; 
extern uint16_t rpm;
extern volatile unsigned long sys_time;
unsigned long sys_time_blink = 0;

uint8_t counter = 0;                //counts transmissions
uint8_t dsp_linecounter = 0;        //counts in which Line we currently are
uint16_t rpm_ranges[12];
uint16_t RPM_LED_Register;
uint16_t RPM_divider = 0;
uint8_t LED_active = 0;



//initbefehle
uint8_t dsp_command [7] = {
	0x39, //function set european chararacter set
	0x08, //display off
	0x06, //entry mode set increment cursor by 1 not shifting display
	0x17, //Character mode and internal power on
	0x00, //0x01, //clear display NEVER USE CLS IT WILL GET THE DISPLAY HOT SICNE EXECUTION TIME >2 ms
	0x02, //return home
	0x0C, //display on
};

uint8_t dsp_line[4] ={
	//This order is known by Trial and Error
	0xC0, //Line 4
	0x94, //Line 2
	0xD4, //Line 3
	0x80, //Line 1
};

//This functions calculates the number of led's that need to be turned on to represent to current motor RPM
void LED_RPM_Bar(){
RPM_divider = RPM_MAX/LED_Count;
if (rpm >= RPM_divider)
{
	LED_active = rpm/RPM_divider;
	LED_active--;
		
	for (RPM_LED_Register=1;LED_active>0;LED_active--){
		RPM_LED_Register = RPM_LED_Register << 1;
		RPM_LED_Register ++;
	}
}
else{
	RPM_LED_Register = 0;
}

};

void RPM_LED_Blink(){

	if (rpm >= shiftrpm & sys_time-sys_time_blink >= Blinkintervall){
		
		if (RPM_LED_PORT_1 == 0){	//when the RPM LED's have been turned off by the blinking algorythm and the intervall is over we want to switch the led's on
			RPM_LED_PORT_1 = RPM_LED_Register;
			RPM_LED_PORT_2 = RPM_LED_Register >> 8;
			sys_time_blink = sys_time;
		}else{						//when the RPM LED's have been turned on by the blinking algorythm and the intervall is over we want to switch the led's off
			RPM_LED_PORT_1 = 0x00;
			RPM_LED_PORT_2 = 0x00;
			sys_time_blink = sys_time;
		}
	}else if(rpm < shiftrpm){							//when the RPM LED's have been turned off by the blinking algorythm and the conditions for the blinking are not met we want to turn them back on again
		RPM_LED_PORT_1 = RPM_LED_Register;
		RPM_LED_PORT_2 = RPM_LED_Register >> 8;		
	}
}


void dsp_writedata(uint8_t data,uint8_t rs) {
	
	//PORTE as Output
	PORTE = data;
	
	//SETS THE RS PIN
	if (rs==1){
	
		PORTC |= (1<<PC0);
	
	} else{
	
		PORTC &= ~(1<<PC0);
	
	}

	//Sets enable high
	PORTC |= (1<<PC2);
	//Sets enable low
	for(int wait = 0; wait<100; wait++){}
	PORTC &= ~(1<<PC2);
	//This is doen so thta the display will read the data pins and process them
};

//Function to set customchars
//charpos = position in cgram
//line 1-8 lines of the char

void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8){
	
	//where to write in cgram
	dsp_writedata(0x40+8*charpos,0);

	//write each line
	dsp_writedata(line1,1);
	dsp_writedata(line2,1);
	dsp_writedata(line3,1);
	dsp_writedata(line4,1);
	dsp_writedata(line5,1);
	dsp_writedata(line6,1);
	dsp_writedata(line7,1);
	dsp_writedata(line8,1);

	//dsiplay return home
	dsp_writedata(0x02,0);
};

void dsp_write(uint8_t dsp_mode){
	
	if (counter<20)
	dsp_writedata(dsp_data[dsp_mode][dsp_linecounter][counter++],1);
	if (counter == 20){

		//go into next line
		dsp_writedata(dsp_line[dsp_linecounter],0);
		counter=0;
		dsp_linecounter+=1;
	}
	if (dsp_linecounter == 4){
		dsp_linecounter = 0;
	}
};

void dsp_definechars()
{
	//definition of needed custom chars
	dsp_customchar(0,0,0,0,0,0,0x1F,0x1F,0x1F);
	dsp_customchar(1,0x1F,0x1F,0,0,0,0,0,0);
	dsp_customchar(2,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF);
	dsp_customchar(3,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E);
	dsp_customchar(4,0xFF,0xFF,0xFF,0xFF,0,0,0,0);
	dsp_customchar(5,0xFF,0xFF,0xFF,0xFF,0xEF,0xEF,0xEF,0xEF);
	dsp_customchar(6,0x08,0x15,0x0A,0x04,0x0A,0x15,0x02,0x00);
	
};

//runs through the init commands
void dsp_init()
{
	//init the display with commands in dsp_command array
	for (int i=0; i<7;i++){
		dsp_writedata(dsp_command[i],0);
	}
}

void selftest(){
	rpm = RPM_MAX;
	long systime_selftest = sys_time;
	uint8_t LEDS_off = 0;
	while (systime_selftest+selftest_time >= sys_time)
	{
		string_to_digit(0,"     DIC V2.0.1     ",0,0);
		string_to_digit(0," Proudly presented  ",0,1);
		string_to_digit(0,"         by         ",0,2);
		string_to_digit(0,"    Lukas Deeken    ",0,3);
		dsp_write(0);
		LED_RPM_Bar();
		RPM_LED_Blink();		
	}
	while (systime_selftest+selftest_time*3 >= sys_time)
	{
		string_to_digit(0,"       Welcome      ",0,0);
		string_to_digit(0,"         to         ",0,1);
		string_to_digit(0,"        TY20        ",0,2);
		string_to_digit(0,"The Last of its Kind",0,3);
		dsp_write(0);
	
		if (systime_selftest+selftest_time+LEDS_off*(selftest_time/LED_Count)<= sys_time )
		{
			RPM_LED_Register = RPM_LED_Register >> 1;
			RPM_LED_PORT_1 = RPM_LED_Register;
			RPM_LED_PORT_2 = RPM_LED_Register >> 8;
			LEDS_off++;
		}
	}
	dsp_clear();
	rpm = 0;
}