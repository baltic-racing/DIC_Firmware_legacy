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
unsigned long time_100Hz = 0;
unsigned long time_50Hz = 0;
uint16_t hv_max = 550;
uint16_t hv_min = 280;
uint8_t direction = 0;
uint16_t led = 0;
uint8_t led_2 = 0;
//uint8_t update_data = 0;
//uint8_t draw_data = 0;

/*	MAIN	*/
int main(void)
{
	/*	Configuration	*/
	port_config();
	sys_timer_config();
	
	//Initialize Display
	dsp_init();
	dsp_clear();
	
	//Enable maskable Interrupts
	sei();
	
	/*	Hardware testing	*/
	selftest();
	
	/*	FOREVER DO	*/
	while(1)
	{
		/*	Code	*/
		selftest();
	}	
}