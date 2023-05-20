/*	INCLUDES	*/ 
#include <avr/io.h>
#include <string.h>

#include "display_functions.h"
#include "display_data.h"

extern uint8_t dsp_data [4][4][20]; 
extern volatile unsigned long sys_time;
long systime_selftest = 0;
uint8_t LEDS_on = 0;


//Definitions
// Data Lines - PORTE
// Enable     - PC2
// RS         - PC0
// RW         - PC1

uint8_t counter = 0;                //counts transmissions
uint8_t dsp_linecounter = 0;        //counts in which Line we currently are

//This array contains commands for initializing the Display

uint8_t dsp_command [7] =
{
	0x39, //function set european character set
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

uint8_t invert_binary_number(uint8_t number_to_invert){
	
	uint8_t out_num = 0;
	
	for (uint8_t it = 0 ; it < 8; it++){

		out_num |= ((number_to_invert>> it) & 1)<<(7-it);
		
	}
	return out_num;
}


//This Function turns off the left Top Bar
void clear_top_left_bar(void)
{
	PORTA &= ~(1<<PA4) & ~(1<<PA5) & ~(1<<PA6) & ~(1<<PA7);
	PORTG &= ~(1<<PG2);
}

//This Function turns off the right Top Bar
void clear_top_right_bar(void)
{
	PORTC = 0;
	PORTG &= ~(1<<PG0) & ~(1<<PG1);
}

//This Function calculates the amount of LEDs of the left Top Bar which needs to be turned on
void led_top_left_bar(uint16_t max_value_l, uint16_t min_value_l, uint16_t current_value_l)
{
	clear_top_left_bar();
	uint16_t Top_Left_Bar_Divider = (max_value_l - min_value_l) / (LED_COUNT_TOP_LEFT - 1);
	int8_t amount_l = (current_value_l - min_value_l + Top_Left_Bar_Divider) / Top_Left_Bar_Divider;
	if (amount_l > 0) {led_left_top_bar_select(amount_l);}
	else {led_left_top_bar_select(0);}
}

//This Function calculates the amount of LEDs of the right Top Bar which needs to be turned on
void led_top_right_bar(uint16_t max_value_r, uint16_t min_value_r, uint16_t current_value_r)
{
	clear_top_right_bar();
	uint16_t Top_Right_Bar_Divider = (max_value_r - min_value_r) / (LED_COUNT_TOP_RIGHT - 1);
	int8_t amount_r = (current_value_r - min_value_r + Top_Right_Bar_Divider) / Top_Right_Bar_Divider;
	if (amount_r > 0) {led_right_top_bar_select(amount_r);}
	else {led_right_top_bar_select(0);}
}

//This Function turns on the desired amount of LEDs of the left Top Bar
void led_left_top_bar_select(uint8_t select_l)
{
	PORTA |= ~(0xFF << select_l) << PA4;
	PORTG |= (1<<PG2) & ((0x80) >> select_l);
}

//This Function turns on the desired amount of LEDs of the right Top Bar
void led_right_top_bar_select(uint8_t select_r)
{
	PORTC |= ~(0xFF >> select_r);
	PORTG |= (~(0xFF << (select_r/9)) << (select_r/9)) + (select_r/10);
}


//This Function writes data into the Display
void dsp_writedata(uint8_t data,uint8_t rs)
{
	//PORTE as Output
	
	PORTE = invert_binary_number(data);
	
	//SETS THE RS PIN
	if (rs==1) {PORTB |= (1<<PB6);}
	else {PORTB &= ~(1<<PB6);}

	//Sets enable high
	PORTB |= (1<<PB4);
	//Sets enable low
	for(int wait = 0; wait<100; wait++){}
	PORTB &= ~(1<<PB4);
	//This is done so that the display will read the data pins and process them
}

//Function to set customchars
//charpos = position in cgram
//line 1-8 lines of the char

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
	for (int i=0; i<7;i++)
	{
		dsp_writedata(dsp_command[i],0);
	}
}

// void Write_LED_Control(uint16_t LED_Register){
// 	uint8_t LED_RegisterLow = 0;
// 	uint8_t LED_RegisterHigh = 0;
// 
// 	//Top bar DDRC komplett + PG0-PG2 + PA4-7
// 	// remember kids think abaut your programming when drawing the schematic for the PCB otherweise this Sh** comes of it
// 	
// 	LED_RegisterLow = LED_Register;
// 	LED_RegisterHigh = LED_Register >> 8;
// 
// 	PORTG = (((LED_RegisterLow & 0b00010000) >> 2) | (PING & 0b11111011));
// 	
// 	PORTG = (invert_binary_number(((LED_RegisterHigh >> 5) & 0b00000011)) >> 6) | (PING & 0b11111100);
// 	
// 	PORTC = invert_binary_number((LED_Register & 0b0001111111100000) >> 5);
// 	
// 	PORTA = ((LED_RegisterLow & 0b00001111) << 4) | (PINA & 0b00001111);
// 
// }

// uint16_t Read_LED_Control(){
// 	uint16_t LED_Register = 0;
// 	
// 	LED_Register = (invert_binary_number((PING & 0b00000011))) << 7;
// 	
// 	LED_Register |= (invert_binary_number(PINC) << 5);
// 	
// 	LED_Register |= ((PING & 0b11111011) << 2);
// 	
// 	LED_Register |= (PINA & 0b00001111) >> 4;
// 	
// 	return LED_Register;
// 	
// }

// void LED_Port_Blink(){
// 	
// 	if (Blink_CTRL = true)
// 	{
// 		if (sys_time-sys_time_blink >= Blinkintervall){
// 		
// 			if (Read_LED_Control == 0){	//when the RPM LED's have been turned off by the blinking algorythm and the intervall is over we want to switch the led's on
// 				Write_LED_Control(0xFFFF);
// 				sys_time_blink = sys_time;
// 				}else{						//when the RPM LED's have been turned on by the blinking algorythm and the intervall is over we want to switch the led's off
// 				Write_LED_Control(0x0000);
// 				sys_time_blink = sys_time;
// 			}
// 		}
// 	}
// }

void selftest(){
	
	if (systime_selftest+selftest_time >= sys_time)
	{
		string_to_digit(0,"      DIC V0.1      ",0,0);
		string_to_digit(0,"  Software written  ",0,1);
		string_to_digit(0,"         by         ",0,2);
		string_to_digit(0,"Baltic Racing Alumni",0,3);
		dsp_write(0);	
	}
	else if (systime_selftest+selftest_time*3 >= sys_time)
	{
		string_to_digit(0,"Initializing Voodoo.",0,0);
		string_to_digit(0," Charging Up Magic. ",0,1);
		string_to_digit(0,"  Filling Up Smoke  ",0,2);
		string_to_digit(0,"  Make 'em WICKED!  ",0,3);
		dsp_write(0);
	}
		
	if (systime_selftest + LEDS_on * (selftest_time/LED_Count+1)*4 < sys_time )
	{
		if (LEDS_on <= LED_COUNT_TOP_LEFT)
		{
			led_left_top_bar_select(LEDS_on);
		}
		else
		{
			led_left_top_bar_select(LED_COUNT_TOP_LEFT);
			led_right_top_bar_select(LEDS_on-LED_COUNT_TOP_LEFT);
		}
		LEDS_on++;
 	}
}