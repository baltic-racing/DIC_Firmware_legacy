/*	INCLUDES	*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "system_config.h"

void _Port_Config_(void)
{
	//PA2 --> IMD_LED, PA3 --> Chip_Select_RGB_Left, (PA4 - PA7) --> Top Bar
	DDRA |= (1<<PA2) | (1<<PA3) | (1<<PA4) | (1<<PA5) | (1<<PA6) | (1<<PA7);
	//Button Inputs
	DDRA &= ~(1<<PA0) & ~(1<<PA1);
	//PB1 --> SCK, PB2 --> MOSI, PB4 --> Display_Enable, PB5 --> Display_R/W, PB6 --> Display_RS, PB7 --> Chip_Select_RGB_Right
	DDRB |= (1<<PB1) | (1<<PB2) |(1<<PB4) | (1<<PB5) | (1<<PB6) | (1<<PB7);
	//PB3 --> MISO
	DDRB &= ~(1<<PB3);
	//Top Bar
	DDRC = 0xFF;
	//Bottom Bar
	DDRD |= (1<<PD0) | (1<<PD1) | (1<<PD2) | (1<<PD3) | (1<<PD4) | (1<<PD7);
	//Display-Bus Bar
	DDRE = 0xFF;
	//SDCI Inputs: PF1 --> SDB_DB, PF2 --> Inertia, PF3 --> BOTS
	DDRF &= ~(1<<PF1) & ~(1<<PF2) & ~(1<<PF3);
	//(PG0 - PG2) --> Top Bar, PG3 --> Heart, PG4 --> Fault
	DDRG |= (1<<PG0) | (1<<PG1) | (1<<PG2) | (1<<PG3) | (1<<PG4);
	
	//Resetting all PORTS
	PORTA = 0;
	PORTB = 0;											//Resetting Display-Control-Ports
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;											//Resetting Display-Bus Bar
	PORTF = 0;
	PORTG = 0;
	
	//Selecting Pull-Up Resistors for Button - Inputs
	PORTA |= (1<<PA0) | (1<<PA1);
}

void _Sys_Timer_Config_(void)
{
	//8 bit Timer 0 configuration
	TCCR0A = 0 | (1<<WGM01) | (1<<CS01) | (1<<CS00);	//Mode --> CTC, Divider --> /64
	TIMSK0 = 0 | (1<<OCIE0A);							//compare interrupt enable
	OCR0A = 250-1;										//compare value for 1ms;
}