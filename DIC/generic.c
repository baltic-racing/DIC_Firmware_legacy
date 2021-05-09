g/*
 * generic.c
 *
 * Created: 03.05.2021 16:36:25
 *  Author: Ole
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>

#include "canlib.h"
#include "generic_defines.h"
#include "display_functions.h"

void Port_init(void)//initialises the needed ports
{
	DDRE = 0xFF;
	DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
	PORTC = 0;
	PORTE = 0;
	
};

void sys_timer_config(){
	//8 bit Timer 0 configuration
	//ctc mode and 64 as prescaler for 16Mhz
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A);	//compare interrupt enable
	OCR0A = 250-1;				// compare value for 1ms;
}