/*	INCLUDES	*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

//#include "CAN_DATA.h"
//#include "canlib.h"
#include "display_data.h"
#include "display_functions.h"
//#include "error.h"
#include "system_config.h"

/*	DEFINES	*/

/*	Init Global Variables	*/
volatile unsigned long sys_time = 0;
volatile uint16_t time_1Hz = 0;
volatile uint16_t time_10Hz = 0;
volatile uint16_t time_100Hz = 0;
uint8_t LED = 0;
uint8_t SWITCH = 0;
//uint8_t update_data = 0;
//uint8_t draw_data = 0;

/*	Interrupt Service Routines	*/
ISR(TIMER0_COMP_vect)
{
	//draw_data = 1;
	//update_data++;
	sys_time++;
	time_1Hz++;
	time_10Hz++;
	time_100Hz++;
	PORTG ^= (1<<PG3);
}

/*	MAIN	*/
int main(void)
{
	_Port_Config_();
	_Sys_Timer_Config_();
	sei();	//Enable maskable Interrupts
	
	/*	FOREVER DO	*/
	while(1)
	{
		if (time_10Hz >= 100)
		{
			if (SWITCH == 0)
			{
				if (LED > 5) {SWITCH = 1;}
				else {LED++;}
				_LED_Top_Left_Bar_(5,0,LED);
				time_10Hz = 0;
			}
			else
			{
				if (LED == 0) {SWITCH = 0;}
				else {LED--;}
				_LED_Top_Left_Bar_(5,0,LED);
				time_10Hz = 0;
			}
		}
	}	
}