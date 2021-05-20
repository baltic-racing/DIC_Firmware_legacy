/*
 * display_data.c
 *
 * Created: 03.05.2021 15:55:28
 *  Author: Ole H.
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>


#include "canlib.h"
#include "generic.h"
#include "display_functions.h"



uint8_t dsp_data [4][4][20]; 
float BB = 0;

uint8_t calc_BB (uint8_t BPF, uint8_t BPR){
	BB = 0;
	if (BPF >= 10){
		BB = BPF+BPR;
		BB = BPF/BB;
		BB = BB*100;
	}
return BB;
}


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


//num_to_3digit this function not only converts the raw data to a 3Digit number which can be displayed onto the display, it also sets the Page and Position of the Number on the Display
void num_to_digit(uint8_t dsp_mode, uint16_t number, uint8_t comma, uint8_t digits, uint8_t offset_column, uint8_t offset_line){
	
	for(uint8_t i = 0; i < digits; i++){
	
		//convertz num to x digits
		uint8_t digit = number % 10;
		digit = 0x30 + digit;
		if (digit == 0x30 & number/10 == 0){ //When number is smaller than digit count for eg 3digit oilt but its only 80°C it only displays 2 digit's
			digit = 0x10;
		}
		number = number/10;

		if(comma == 1){
		
			if(i < 1){
				dsp_data [dsp_mode][offset_line][offset_column+digits-i] = digit;
				dsp_data [dsp_mode][offset_line][offset_column+digits-1-i] = '.';
			}else{
				dsp_data [dsp_mode][offset_line][offset_column+digits-1-i] = digit;
			}
		
		} else {
			
			dsp_data [dsp_mode][offset_line][offset_column+digits-1-i] = digit;
		}
	}
};

void time_to_digit(uint8_t dsp_mode, uint32_t number, uint8_t offset_column, uint8_t offset_line){
	
//convertz number to time format m:ss:msmsms
uint16_t milliseconds = number % 1000;
uint16_t minutes = number / 1000;
minutes = minutes / 60;
uint16_t seconds = number / 1000;
seconds = seconds % 60;

	//milliseconds conversion
	for(uint8_t i = 0; i < 3; i++){
		uint8_t digit = milliseconds % 10;
		digit = 0x30 + digit;
		milliseconds = milliseconds/10;
			
		dsp_data [dsp_mode][offset_line][offset_column+8-1-i] = digit;
	}
	//seconds conversion
	for(uint8_t i = 0; i < 2; i++){
		uint8_t digit = seconds % 10;
		digit = 0x30 + digit;
		seconds = seconds/10;
		
		dsp_data [dsp_mode][offset_line][offset_column+4-1-i] = digit;
	}
	//minutes conversion
	for(uint8_t i = 0; i < 2; i++){
		uint8_t digit = minutes % 10;
		digit = 0x30 + digit;
		if (digit == 0x30 & minutes/10 == 0){ //When number is smaller than digit count for eg 3digit oilt but its only 80°C it only displays 2 digit's
			digit = 0x10;
		}
		minutes = minutes/10;
		
		dsp_data [dsp_mode][offset_line][offset_column+1-1-i] = digit;
	}	
	
};

void dsp_arrayinit(void)
{

	//init the Display with and empty char
	for (int i = 0; i<4;i++){
		for (int s=0;s<20;s++){
			dsp_data[0][i][s]=0x10;
			dsp_data[1][i][s]=0x10;
			dsp_data[2][i][s]=0x10;
			dsp_data[3][i][s]=0x10;
		}
	}
//OLD Display implementation
	//STATUSINDICATOR
	/*
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
	*/
//new home screen
	//first argument is the display mode here i is always 0, second number is the row count (4 rows 1-3), 3argument is the Line count 0-20
	//Names
	dsp_data[0][0][0] = 'C';
	dsp_data[0][0][1] = 'O';
	dsp_data[0][0][2] = 'O';
	dsp_data[0][0][3] = 'L';
	
	dsp_data[0][1][0] = 'O';
	dsp_data[0][1][1] = 'E';
	dsp_data[0][1][2] = 'L';	
	
	dsp_data[0][2][0] = 'B';
	dsp_data[0][2][1] = 'B';	
	
	dsp_data[0][2][7] = 'C';
	dsp_data[0][2][8] = 'L';
	dsp_data[0][2][9] = 'U';
	//Units
	dsp_data[0][1][7] = 'B';
	
	dsp_data[0][0][15] = 'C';
	dsp_data[0][1][15] = 'C';
	dsp_data[0][2][15] = 's';
	dsp_data[0][3][15] = 'V';
	
//DEBUG INTERFACE
	//Names
	dsp_data[1][0][0] = 'T'; 
	dsp_data[1][0][1] = 'P';
	dsp_data[1][0][2] = 'S';
	dsp_data[1][0][3] = '1';
	dsp_data[1][0][10] = '2';
	
	dsp_data[1][1][0] = 'A';
	dsp_data[1][1][1] = 'P';
	dsp_data[1][1][2] = 'S';
	dsp_data[1][1][3] = '1';
	dsp_data[1][1][10] = '2';
	
	dsp_data[1][2][0] = 'T';
	dsp_data[1][2][1] = 'P';
	dsp_data[1][2][2] = 'S';
	dsp_data[1][2][3] = 'E';	
	
	dsp_data[1][3][0] = 'B';
	dsp_data[1][3][1] = 'P';
	dsp_data[1][3][2] = 'F';
	dsp_data[1][3][8] = 'R';	
	//Units
	dsp_data[1][0][8] = '%';
	dsp_data[1][1][8] = '%';
	dsp_data[1][2][8] = '%';		
	
	dsp_data[1][0][15] = '%';
	dsp_data[1][1][15] = '%';	
	
	dsp_data[1][3][6] = 'B';
	dsp_data[1][3][12] = 'B';	
	
//Timing screen

	dsp_data[2][0][0] = 'T';
	dsp_data[2][0][1] = 'C';
	
	dsp_data[2][1][0] = 'B';
	dsp_data[2][1][1] = 'E';
	dsp_data[2][1][2] = 'S';		
	
	dsp_data[2][2][0] = 'P';
	dsp_data[2][2][1] = 'R';
	dsp_data[2][2][2] = 'E';
	
	dsp_data[2][3][0] = 'L';
	dsp_data[2][3][1] = 'N';
	
	dsp_data[2][0][5] = 'S';
	dsp_data[2][0][6] = 'P';	
	dsp_data[2][0][7] = 'E';
	dsp_data[2][0][8] = 'D';
	
	dsp_data[2][0][13]= 'K';
	dsp_data[2][0][14]= 'M';
	dsp_data[2][0][15]= 'H';	

	dsp_data[2][3][6]= 'O';
	dsp_data[2][3][7]= 'D';
	dsp_data[2][3][8]= 'O';
	
	dsp_data[2][3][13] = 'K';
	dsp_data[2][3][14] = 'M';	
	
	dsp_data[2][1][5] = ':';
	dsp_data[2][1][8] = ':';
	dsp_data[2][2][5] = ':';
	dsp_data[2][2][8] = ':';
	
		
};



