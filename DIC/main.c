
// dic.c
//
// Created: 13.03.2018 10:58:20
// Author : Ole Hannemann

//General Information on the Software Code, basically where does information come from and go to
/*
CAN_Data.c & CAN_Data.h
	here you get all your data from. in these files you can add structs for CAN ID`s and define your variables in reference to their Position in the CAN message. 
	So here is the source of ALL your data being displayed.
	
canlib.c & canlib.h
	These files contain functions relevant to the CAN communication, Generally you will not have to mess with these. Everything regarding can you will have to dealk with is located 
	in the CAN_data files
	
display_data.c & display_data.h
	These Files contain functions for converting your data to a format that it can easily displayed on the display. 
	Additionally a function for displaying large lumbers such as with the gear indicator is included
	
display_functions.c & display_functions.h
	here you find functions for actually writing the data to display over the parralel bus, the custom caracters are defined here and also things that are used around the generall 
	functionality of the display like the RPM Bar and the Shutdown Leds
	
generic.c & generic.h
	in here you will find all the non specific functions, like the Port initialization and timer configs and all that kind of stuff
*/

//TODO list
/*								Implemented		Checked		Tested
Write char function					X
Version info function				X
goto position function				X
blinking Cursor (Optional)
Selftest
hardware LED RPM indicator
shutdown LED indicator
Error detection
error indication
error LED blink
timing display
debug display
home display
large RPM display
TC RPM BAR blink
TC adjustment
All Numbers as Custom Char
CAN Error Check check for non zero can messages
*/

//Definitions
// Data Lines - PORTE
// Enable     - PC2
// RS         - PC0
// RW         - PC1
#include <avr/interrupt.h>
#include <avr/io.h>

#include "canlib.h"
#include "generic.h"
#include "display_functions.h"


uint8_t dsp_mde = 1;
uint8_t update_data = 1;
uint8_t draw_data = 1;

//Volatiles -> Volatile indicates the compiler that the variable might change value unexpectatly for e.g. throug an Interrupt. 
//this guarantees that the variable will always be loaded from memory when used
volatile uint8_t time_100 = 0;
volatile unsigned long sys_time = 0;

//Global Variables -> extern indicates that variables are defined in another File always use extern except the file where the Variabel is first deklared
extern uint8_t Rotary_Encoder_Right;

ISR(TIMER0_COMP_vect)
{ //For every interrupt (1ms/1kHz) write one character into the display
	draw_data = 1;
	update_data++;
	sys_time++;
}

// MAIN FUNCTION
int main(void)
{
	//Init and config part
	Port_init();
	can_init_messages();
	dsp_init();
	dsp_arrayinit();
	dsp_definechars();
	sys_timer_config();
	can_cfg();

	sei();

	//Loop
	while(1){

		if(update_data>=33){ //refresh rate for display about 30Hz
			update_data = 0;
			if (dsp_mde == 1){ //DSP Mode 1 = Home
				//Put numto3digit here with the neccessary information required for thepage
			}
			else { //implement switch case for the different pages and modify ds_data_array

			}
		}
		if(draw_data){ //1000Hz/1ms loop
			dsp_write(Rotary_Encoder_Right);
			draw_data = 0;
		}	
		if(sys_time - time_100 >= 10){//100Hz/10ms loop
			CAN_recieve();
			time_100 = sys_time;
			
		}
	}	
}