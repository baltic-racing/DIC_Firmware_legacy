
// dic.c
//
// Created: 13.03.2018 10:58:20
// Author : Ole Hannemann


//Definitions
// Data Lines - PORTE
// Enable     - PC2
// RS         - PC0
// RW         - PC1
#include <avr/interrupt.h>
#include <avr/io.h>

#include "canlib.h"
#include "generic_defines.h"
#include "display_functions.h"

//DEFINES
#define rx_mobs 7

//VARIABLES

uint8_t can_data_bytes[rx_mobs][8];

void port_config();

uint8_t dsp_mde = 1;

uint8_t update_data = 1;
uint8_t draw_data = 1;

ISR(TIMER1_COMPA_vect)
{ //For every interrupt write one character into the display
	draw_data = 1;
	update_data++;
}

 void timer_config(void)
{
	//TIMER CONFIG
	TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
	TIMSK1 |= (1<<OCIE1A);
	//start with a logical hightime
	OCR1A = 15;
}
//GENERAL FUNCTIONS



// MAIN FUNCTION

int main(void)
{
	//Init and config part
	initPorts();
	dsp_initdsp();
	dsp_arrayinit();
	dsp_definechars();
	timer_config();
	can_cfg();
	sei();

	//Loop
	while(1){

		if(update_data>=44){
			update_data = 0;
			//dsp_tps_err();
			//dsp_mode();
			if (dsp_mde == 1){ //DSP Mode 1 = Home
				//dsp_rpm(); //change to hardware LED`S
				//dsp_status(); //Refresh Sensor Values on the Display
				//dsp_tcs(); unused Traction control code
			}
			else { //else Debug/Sensor Mode
				//dsp_break_temp();
				//dsp_voltage();
				//dsp_break_pres();
				//dsp_oil_temp();
			}
		}
		if(draw_data){
			dsp_write(swc_mob[0]);
			draw_data = 0;
		}	
	}	
}