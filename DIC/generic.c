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

void initPorts(void)                                                 //initialises the needed ports
{
	DDRE = 0xFF;
	DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
	PORTC = 0;
	PORTE = 0;
	
};