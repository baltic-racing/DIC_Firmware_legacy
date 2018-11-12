/*
 * test-display.c
 *
 * Created: 11.04.2018 20:51:54
 * Author : ole
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t dsp_command [7] = {
	0x39, //function set european chararacter set
	0x08, //display off
	0x06, //entry mode set increment cursor by 1 not shifting display
	0x17, //Character mode and internal power on
	0x00, //0x01, //clear display NEVER USE CLS IT WILL GET THE DISPLAY HOT SICNE EXECUTION TIME >2 ms
	0x02, //return home
	0x0C, //display on
};

void initPorts(void)                                                 //initialises the needed ports
{
	DDRE = 0xFF;
	DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
	PORTC = 0;
	PORTE = 0;
	
};

void dsp_writedata(uint8_t data,uint8_t rs) //writes data on the 8 Datapins for the 8 bit communication and the rs Pins
{
	//PORTE as Output
	PORTE = data;
	
	//SETS THE RS PIN
	if (rs==1)
	PORTC |= (1<<PC0);
	else
	PORTC &= ~(1<<PC0);


	//Sets enable high
	PORTC |= (1<<PC2);
	//Sets enable low
	for(int wait = 0; wait<100; wait++){}
	PORTC &= ~(1<<PC2);
	//This is doen so thta the display will read the data pins and process them
}

void dsp_initdsp()
{
	//init the display with commands in dsp_command array
	for (int i=0; i<7;i++){
		dsp_writedata(dsp_command[i],0);
	}
	
}


int main(void)
{
    /* Replace with your application code */
	initPorts();
	dsp_initdsp();

    while (1) 
    {

	dsp_writedata('A',1);
    }
}

