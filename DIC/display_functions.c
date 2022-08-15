/*	INCLUDES	*/ 
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#include "display_functions.h"
#include "display_data.h"

extern uint8_t dsp_data [4][4][20]; 
extern volatile unsigned long sys_time;
//unsigned long sys_time_blink = 0;

uint8_t counter = 0;                //counts transmissions
uint8_t dsp_linecounter = 0;        //counts in which Line we currently are
uint16_t Top_Left_Bar_Divider = 0;
uint16_t Top_Right_Bar_Divider = 0;
//uint8_t LED_active = 0;
//uint8_t Shutdown_byte = 0;

//This array contains commands for initializing the Display
uint8_t dsp_command [7] =
{
	0x39, //function selects European character set
	0x08, //display off
	0x06, //entry mode set increment cursor by 1 not shifting display
	0x17, //Character mode and internal power on
	0x00, //0x01, //clear display NEVER USE CLS IT WILL GET THE DISPLAY HOT SINCE EXECUTION TIME >2 ms
	0x02, //return home
	0x0C, //display on
};

//This array contains the addresses of the display lines
uint8_t dsp_line [4] =
{
	//This order is known by Trial and Error
	0xC0, //Line 4
	0x94, //Line 2
	0xD4, //Line 3
	0x80, //Line 1
};

//This Function turns off the left Top Bar
void _Clear_Top_Left_Bar_(void)
{
	PORTA &= ~(1<<PA4) & ~(1<<PA5) & ~(1<<PA6) & ~(1<<PA7);
	PORTG &= ~(1<<PG2);
}
//This Function turns off the right Top Bar
void _Clear_Top_Right_Bar_(void)
{
	PORTC = 0;
	PORTG &= ~(1<<PG0) & ~(1<<PG1);
}

//This Function calculates the amount of LEDs of the left Top Bar which needs to be turned on
void _LED_Top_Left_Bar_(uint16_t max_value_l, uint16_t min_value_l, uint16_t current_value_l)
{
	_Clear_Top_Left_Bar_();
	Top_Left_Bar_Divider = (max_value_l - min_value_l)/LED_COUNT_Top_LEFT;
	if		((current_value_l >= (Top_Left_Bar_Divider * 1)) && (current_value_l < (Top_Left_Bar_Divider * 2))) {_LED_Left_Top_Bar_Select_(1);}
	else if	((current_value_l >= (Top_Left_Bar_Divider * 2)) && (current_value_l < (Top_Left_Bar_Divider * 3))) {_LED_Left_Top_Bar_Select_(2);}
	else if	((current_value_l >= (Top_Left_Bar_Divider * 3)) && (current_value_l < (Top_Left_Bar_Divider * 4))) {_LED_Left_Top_Bar_Select_(3);}
	else if	((current_value_l >= (Top_Left_Bar_Divider * 4)) && (current_value_l < (Top_Left_Bar_Divider * 5))) {_LED_Left_Top_Bar_Select_(4);}
	else if	 (current_value_l >= (Top_Left_Bar_Divider * 5))													{_LED_Left_Top_Bar_Select_(5);}
	else																										{_LED_Left_Top_Bar_Select_(0);}
}

//This Function calculates the amount of LEDs of the right Top Bar which needs to be turned on
void _LED_Top_Right_Bar_(uint16_t max_value_r, uint16_t min_value_r, uint16_t current_value_r)
{
	_Clear_Top_Right_Bar_();
	Top_Right_Bar_Divider = (max_value_r - min_value_r)/LED_COUNT_Top_RIGHT;
	if		((current_value_r >= (Top_Right_Bar_Divider * 1)) && (current_value_r < (Top_Right_Bar_Divider * 2)))	{_LED_Right_Top_Bar_Select_(1);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 2)) && (current_value_r < (Top_Right_Bar_Divider * 3)))	{_LED_Right_Top_Bar_Select_(2);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 3)) && (current_value_r < (Top_Right_Bar_Divider * 4)))	{_LED_Right_Top_Bar_Select_(3);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 4)) && (current_value_r < (Top_Right_Bar_Divider * 5)))	{_LED_Right_Top_Bar_Select_(4);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 5)) && (current_value_r < (Top_Right_Bar_Divider * 6)))	{_LED_Right_Top_Bar_Select_(5);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 6)) && (current_value_r < (Top_Right_Bar_Divider * 7)))	{_LED_Right_Top_Bar_Select_(6);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 7)) && (current_value_r < (Top_Right_Bar_Divider * 8)))	{_LED_Right_Top_Bar_Select_(7);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 8)) && (current_value_r < (Top_Right_Bar_Divider * 9)))	{_LED_Right_Top_Bar_Select_(8);}
	else if	((current_value_r >= (Top_Right_Bar_Divider * 9)) && (current_value_r < (Top_Right_Bar_Divider * 10)))	{_LED_Right_Top_Bar_Select_(9);}
	else if	 (current_value_r >= (Top_Right_Bar_Divider * 10))														{_LED_Right_Top_Bar_Select_(10);}
	else																											{_LED_Right_Top_Bar_Select_(0);}
}

//This Function turns on the desired amount of LEDs of the left Top Bar
void _LED_Left_Top_Bar_Select_(uint8_t select_l)
{
	switch (select_l)
	{
		case 0: _Clear_Top_Left_Bar_(); break;
		case 1: PORTA |= (1<<PA4); break;
		case 2: PORTA |= (1<<PA4) | (1<<PA5); break;
		case 3: PORTA |= (1<<PA4) | (1<<PA5) | (1<<PA6); break;
		case 4: PORTA |= (1<<PA4) | (1<<PA5) | (1<<PA6) | (1<<PA7); break;
		case 5: PORTA |= (1<<PA4) | (1<<PA5) | (1<<PA6) | (1<<PA7); PORTG |= (1<<PG2); break;
	}
}

//This Function turns on the desired amount of LEDs of the right Top Bar
void _LED_Right_Top_Bar_Select_(uint8_t select_r)
{
	switch (select_r)
	{
		case 0: _Clear_Top_Right_Bar_(); break;
		case 1:  PORTC |= (1<<PC7); break;
		case 2:  PORTC |= (1<<PC7) | (1<<PC6); break;
		case 3:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5); break;
		case 4:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4); break;
		case 5:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4) | (1<<PC3); break;
		case 6:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4) | (1<<PC3) | (1<<PC2); break;
		case 7:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4) | (1<<PC3) | (1<<PC2) | (1<<PC1); break;
		case 8:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4) | (1<<PC3) | (1<<PC2) | (1<<PC1) | (1<<PC0); break;
		case 9:  PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4) | (1<<PC3) | (1<<PC2) | (1<<PC1) | (1<<PC0); PORTG |= (1<<PG1); break;
		case 10: PORTC |= (1<<PC7) | (1<<PC6) | (1<<PC5) | (1<<PC4) | (1<<PC3) | (1<<PC2) | (1<<PC1) | (1<<PC0); PORTG |= (1<<PG1) | (1<<PG0); break;
	}
}

//This Function writes data into the Display
void dsp_writedata(uint8_t data,uint8_t rs)
{
	//PORTE as Output
	PORTE = data;
	
	//SETS THE RS PIN
	if (rs==1) {PORTB |= (1<<PB6);}
	else {PORTB &= ~(1<<PB6);}

	//Sets enable high
	PORTB |= (1<<PB4);
	//Sets enable low
	for(int wait = 0; wait<100; wait++){}
	PORTB &= ~(1<<PB4);
	//This is doen so that the display will read the data pins and process them
}

//Function to set customchars
//charpos = position in cgram
//line 1-8 lines of the char
/*
void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8)
{	
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
}
*/

void dsp_write(uint8_t dsp_mode)
{	
	if (counter<20)
	dsp_writedata(dsp_data[dsp_mode][dsp_linecounter][counter++],1);
	if (counter == 20)
	{
		//go into next line
		dsp_writedata(dsp_line[dsp_linecounter],0);
		counter=0;
		dsp_linecounter+=1;
	}
	if (dsp_linecounter == 4)
	{
		dsp_linecounter = 0;
	}
}

/*
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
*/
//runs through the init commands

void dsp_init()
{
	//init the display with commands in dsp_command array
	for (int i=0; i<7;i++)
	{
		dsp_writedata(dsp_command[i],0);
	}
}


void selftest()
{
	long systime_selftest = sys_time;
	uint8_t LEDS_off = 0;
	while (systime_selftest+selftest_time >= sys_time)
	{
		string_to_digit(0,"      DIC V0.1      ",0,0);
		string_to_digit(0,"  Software written  ",0,1);
		string_to_digit(0,"         by         ",0,2);
		string_to_digit(0,"   Baltic Racing!   ",0,3);
		dsp_write(0);	
	}
	while (systime_selftest+selftest_time*3 >= sys_time)
	{
		string_to_digit(0,"Initializing Voodoo.",0,0);
		string_to_digit(0," Charging Up Magic. ",0,1);
		string_to_digit(0,"Filling Up Gasoline.",0,2);
		string_to_digit(0,"  Make 'em WICKED!  ",0,3);
		dsp_write(0);
	}
	dsp_clear();
}
