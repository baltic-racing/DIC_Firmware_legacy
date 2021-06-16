/*
 * generic.c
 *
 * Created: 03.05.2021 16:36:25
 *  Author: Ole
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>


#include "canlib.h"
#include "generic.h"
#include "display_functions.h"

void Port_init(void)//initialises the needed ports
{
	DDRA = 0xFF; //Set PORTA as Output (RPM BAR)
	DDRB = 0xFF; //Set PORTB as Output (RPM BAR)
	DDRB &= ~(1<<PB7); //Reset PINB7 as Input as it is not used in the RPM BAR
	DDRE = 0xFF;
	DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
	DDRD = 0 & ~(1<<PD0) & ~(1<<PD1) & ~(1<<PD2) & ~(1<<PD3) & ~(1<<PD4) & ~(1<<PD7); 
	PORTC = 0;
	PORTE = 0;
	DDRG = 0 | (1<<PG0) | (1<< PG1);
	DDRF = 0 | (1<<PF0) | (1<<PF1) | (1<<PF2) | (1<<PF3);
};

void sys_timer_config(){
	//8 bit Timer 0 configuration
	//ctc mode and 64 as prescaler for 16Mhz
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);
	TIMSK0 = 0 | (1<<OCF0A);	//compare interrupt enable
	OCR0A = 250-1;				// compare value for 1ms;
}


