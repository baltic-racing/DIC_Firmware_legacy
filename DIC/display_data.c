/*
 * display_data.c
 *
 * Created: 03.05.2021 15:55:28
 *  Author: Ole H.
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>

#include "canlib.h"
#include "generic_defines.h"
#include "display_functions.h"



uint8_t dsp_data [2][4][20]; 



void large_number(uint8_t dsp_mode, uint8_t offset, uint8_t number){

	switch (number){

		//NEUTRAL
		case 0:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;

		//second Line
		dsp_data[dsp_mode][1][offset] = 0xFF;
		dsp_data[dsp_mode][1][offset+1] = 0x10;
		dsp_data[dsp_mode][1][offset+2] = 0xFF;

		//third Line
		dsp_data[dsp_mode][2][offset] = 0xFF;
		dsp_data[dsp_mode][2][offset+1] = 0x10;
		dsp_data[dsp_mode][2][offset+2] = 0xFF;

		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0xFF;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;

		//First Gear
		case 1:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0x10;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;
		
		//second Line
		dsp_data[dsp_mode][1][offset] = 0x10;
		dsp_data[dsp_mode][1][offset+1] = 0x10;
		dsp_data[dsp_mode][1][offset+2] = 0xFF;
		
		//third Line
		dsp_data[dsp_mode][2][offset] = 0x10;
		dsp_data[dsp_mode][2][offset+1] = 0x10;
		dsp_data[dsp_mode][2][offset+2] = 0xFF;
		
		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0x10;
		dsp_data[dsp_mode][3][offset+1] = 0x10;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;

		//Second Gear
		case 2:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;
		
		//second Line
		dsp_data[dsp_mode][1][offset] = 0x00; //custom char 1
		dsp_data[dsp_mode][1][offset+1] = 0x00; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0xFF;
		
		//third Line
		dsp_data[dsp_mode][2][offset] = 0xFF;
		dsp_data[dsp_mode][2][offset+1] = 0x01; //custom char 2
		dsp_data[dsp_mode][2][offset+2] = 0x01; //custom char 2
		
		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0xFF;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;
		
		//Third Gear
		case 3:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;
		
		//second Line
		dsp_data[dsp_mode][1][offset] = 0x00; //custom char 1
		dsp_data[dsp_mode][1][offset+1] = 0x00; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0xFF;
		
		//third Line
		dsp_data[dsp_mode][2][offset] = 0x01; //custom char 2
		dsp_data[dsp_mode][2][offset+1] = 0x01; //custom char 2
		dsp_data[dsp_mode][2][offset+2] = 0xFF;
		
		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0xFF;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;

		//Fourth Gear
		case 4:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0x03; //custom char 4
		dsp_data[dsp_mode][0][offset+1] = 0x10;
		dsp_data[dsp_mode][0][offset+2] = 0x10;
		
		//second Line
		dsp_data[dsp_mode][1][offset] = 0x03; //custom char 4
		dsp_data[dsp_mode][1][offset+1] = 0x02; //custom char 3
		dsp_data[dsp_mode][1][offset+2] = 0x10;
		
		//third Line
		dsp_data[dsp_mode][2][offset] = 0x04; //custom char 5
		dsp_data[dsp_mode][2][offset+1] = 0x05; //custom char 6
		dsp_data[dsp_mode][2][offset+2] = 0x04; //custom char 5
		
		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0x10;
		dsp_data[dsp_mode][3][offset+1] = 0x02; //custom char 3
		dsp_data[dsp_mode][3][offset+2] = 0x10;
		break;

		//Fith Gear
		case 5:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;
		
		//second Line
		dsp_data[dsp_mode][1][offset] = 0xFF;
		dsp_data[dsp_mode][1][offset+1] = 0x00; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0x00; //custom char 1
		
		//third Line
		dsp_data[dsp_mode][2][offset] = 0x01; //custom char 2
		dsp_data[dsp_mode][2][offset+1] = 0x01; //custom char 2
		dsp_data[dsp_mode][2][offset+2] = 0xFF;
		
		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0xFF;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;

		//6th Gear
		case 6:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;
		
		//second Line
		dsp_data[dsp_mode][1][offset] = 0xFF;
		dsp_data[dsp_mode][1][offset+1] = 0x00; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0x00; //custom char 1
		
		//third Line
		dsp_data[dsp_mode][2][offset] = 0xFF;
		dsp_data[dsp_mode][2][offset+1] = 0x10;
		dsp_data[dsp_mode][2][offset+2] = 0xFF;
		
		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0xFF;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;
		
		//if no valid gear was transmitted
		default:
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0xFF;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;
		
		dsp_data[dsp_mode][1][offset] = 0xFF;
		dsp_data[dsp_mode][1][offset+1] = 0xFF;
		dsp_data[dsp_mode][1][offset+2] = 0xFF;
		
		dsp_data[dsp_mode][2][offset] = 0xFF;
		dsp_data[dsp_mode][2][offset+1] = 0xFF;
		dsp_data[dsp_mode][2][offset+2] = 0xFF;
		
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0xFF;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;

		
	}

	
};

void num_to_3digit(uint8_t dsp_mode, uint16_t number, uint8_t comma, uint8_t digits, uint8_t offset_w, uint8_t offset_l){
	
	
	
	for(uint8_t i = 0; i < digits; i++){
	
		//convertz num to x digits
		uint8_t digit = number % 10;
		digit = 0x30 + digit;
		number = number/10;
		
		if(comma == 1){
		
			if(i < 1){
				dsp_data [dsp_mode][offset_l][offset_w+digits-i] = digit;
				dsp_data [dsp_mode][offset_l][offset_w+digits-1-i] = '.';
			}else{
				dsp_data [dsp_mode][offset_l][offset_w+digits-1-i] = digit;
			}
		
		} else {
			
			dsp_data [dsp_mode][offset_l][offset_w+digits-1-i] = digit;
			
		}
	}
	
	
	
};

void dsp_arrayinit(void)
{

	//init the Display with and empty char
	for (int i = 0; i<4;i++){
		for (int s=0;s<20;s++){
			dsp_data[0][i][s]=0x10;
			dsp_data[1][i][s]=0x10;
		}
	}

	//STATUSINDICATOR
	// writes =
	dsp_data[1][3][1] = '=';
	dsp_data[1][3][8] = '=';
	dsp_data[1][2][6] = '=';
	dsp_data[1][2][10] = '=';
	dsp_data[1][2][14] = '=';
	
	// writes C for coolingtemparature
	dsp_data[1][3][0] = 'C';
	// writes O
	dsp_data[1][3][7] = 'O';
	// writes C
	dsp_data[1][3][5] =  'C';
	// writes B
	dsp_data[1][3][13] = 'B';
	// writes .
	dsp_data[1][3][10] = '.';
	//writes a T for TCS
	dsp_data[1][2][9] = 'T';
	//writes a E for error LED
	dsp_data[1][2][13] = 'E';
	//0 for RPM
	dsp_data[1][2][0]= 0x30;
	dsp_data[1][2][1]= 0x30;
	dsp_data[1][2][2]= 0x30;
	dsp_data[1][2][3]= 0x30;
	
	//DEBUG INTERFACE
	dsp_data[0][0][0] = 'B'; dsp_data[0][0][1] = 'P'; dsp_data[0][0][2] = 'F'; dsp_data[0][0][3] = 'R'; dsp_data[0][0][4] = '='; dsp_data[0][0][7] = 'B';	//implemented
	dsp_data[0][1][0] = 'B'; dsp_data[0][1][1] = 'P'; dsp_data[0][1][2] = 'R'; dsp_data[0][1][3] = 'E'; dsp_data[0][1][4] = '='; dsp_data[0][1][7] = 'B';	//implemented
	dsp_data[0][2][0] = 'B'; dsp_data[0][2][1] = 'T'; dsp_data[0][2][2] = 'L'; dsp_data[0][2][3] = '='; dsp_data[0][2][7] = 'C';							//implemented
	dsp_data[0][3][0] = 'B'; dsp_data[0][3][1] = 'T'; dsp_data[0][3][2] = 'R'; dsp_data[0][3][3] = '='; dsp_data[0][3][7] = 'C';							//implemented
	
	dsp_data[0][0][9] = 'V'; dsp_data[0][0][10] = 'B'; dsp_data[0][0][11] = '='; dsp_data[0][0][14] = '.'; dsp_data[0][0][16] = 'V';						//implemented
	dsp_data[0][1][9] = 'T'; dsp_data[0][1][10] = 'P'; dsp_data[0][1][11] = 'S'; dsp_data[0][1][12] = '='; dsp_data[0][1][16] = 0x06; 					//implemented
	dsp_data[0][2][9] = 'O'; dsp_data[0][2][10] = 'i'; dsp_data[0][2][11] = 'T'; dsp_data[0][2][12] = '='; dsp_data[0][2][16] = 'C';            //implemented
	dsp_data[0][3][9] = 'S'; dsp_data[0][3][10] = 'l'; dsp_data[0][3][11] = 'i'; dsp_data[0][3][12] = '='; dsp_data[0][3][16] = 0x06;
	
};



