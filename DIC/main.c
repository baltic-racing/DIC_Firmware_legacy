
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
function num_to_digit displays zero when value is smaller than max digit count. needs to be acounted for -- Implemented
function num_to_digit when value larger than digit is put in it cuts the excess instead of showing for e.g. 999
Implement Brake Bias Calculation
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


uint8_t dsp_mde = 2;
uint8_t update_data = 1;
uint8_t draw_data = 1;
extern uint8_t dsp_data [4][4][20];

//Volatiles -> Volatile indicates the compiler that the variable might change value unexpectatly for e.g. throug an Interrupt. 
//this guarantees that the variable will always be loaded from memory when used
volatile uint8_t time_100 = 0;
volatile unsigned long sys_time = 0;

//Global Variables -> extern indicates that variables are defined in another File always use extern except the file where the Variabel is first deklared
extern uint8_t Rotary_Encoder_Right;
extern uint8_t gear;
extern uint8_t CLT;
extern uint8_t OILP;
extern uint8_t OILT;
extern uint8_t BrakeBias;
extern uint8_t Clutchtime;
extern uint8_t ECUVoltage;


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
			if (dsp_mde == 0){ //DiSPlay Mode 0 = Home
				large_number(0,17,gear);//Large Number for gear indicator
				num_to_digit(0,CLT,0,3,11,0);//writes the number 121 for clt TEST ONLY!!!
				num_to_digit(0,OILT,0,3,11,1);//writes the number 113 for OilT TEST ONLY!!!
				num_to_digit(0,OILP,1,2,4,1);//writes the number 5,6 for oil Pressure TEST ONLY!!!
				num_to_digit(0,BrakeBias,0,2,4,2);//writes the number 55 for BB  TEST ONLY!!!
				num_to_digit(0,Clutchtime,1,2,11,2);//writes the number 1,5 for CLU  TEST ONLY!!!
				num_to_digit(0,ECUVoltage,1,3,10,3);//writes the number 12,4 for Voltage TEST ONLY!!!
				if (Clutchtime == 0)
				{ 
					dsp_data[0][2][7] = 'L';
					dsp_data[0][2][8] = 'C';
					dsp_data[0][2][9] = 0x10;
					dsp_data[0][2][10] = 'A';
					dsp_data[0][2][11] = 'C';
					dsp_data[0][2][12] = 'T';	
					dsp_data[0][2][13] = 'I';
					dsp_data[0][2][14] = 'V';
					dsp_data[0][2][15] = 'E';				
				}
			}
			if (dsp_mde == 1){ //debug screen
				large_number(1,17,gear);//Large Number for gear indicator
				
			}
			if (dsp_mde == 2){ //Times screen
				large_number(2,17,gear);//Large Number for gear indicator
				
			}
			if (dsp_mde == 3){ //RPM
				
			}			
		}
		if(draw_data){ //1000Hz/1ms loop
			dsp_write(dsp_mde);
			draw_data = 0;
		}	
		if(sys_time - time_100 >= 10){//100Hz/10ms loop
			CAN_recieve();
			time_100 = sys_time;
			
		}
	}	
}