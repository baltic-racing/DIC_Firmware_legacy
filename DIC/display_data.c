/*	INCLUDES	*/ 
#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

//#include "canlib.h"
#include "system_config.h"
#include "display_functions.h"

uint8_t dsp_data [4][4][20]; 
float BB = 0;


uint8_t calc_BB (uint8_t BPF, uint8_t BPR){
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
		
		case 7:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
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
		
		case 8:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0x01;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;

		//second Line
		dsp_data[dsp_mode][1][offset] = 0xFF;
		dsp_data[dsp_mode][1][offset+1] = 0x07; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0xFF; //custom char 1

		//third Line
		dsp_data[dsp_mode][2][offset] = 0xFF;
		dsp_data[dsp_mode][2][offset+1] = 0x01;
		dsp_data[dsp_mode][2][offset+2] = 0xFF;

		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0x07;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;
		
		case 9:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0xFF;
		dsp_data[dsp_mode][0][offset+1] = 0x01;
		dsp_data[dsp_mode][0][offset+2] = 0xFF;

		//second Line
		dsp_data[dsp_mode][1][offset] = 0xFF;
		dsp_data[dsp_mode][1][offset+1] = 0x07; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0xFF; //custom char 1

		//third Line
		dsp_data[dsp_mode][2][offset] = 0x10;
		dsp_data[dsp_mode][2][offset+1] = 0x01;
		dsp_data[dsp_mode][2][offset+2] = 0xFF;

		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0xFF;
		dsp_data[dsp_mode][3][offset+1] = 0x07;
		dsp_data[dsp_mode][3][offset+2] = 0xFF;
		break;
		
		//empty char
		case 10:
		//First Line
		dsp_data[dsp_mode][0][offset] = 0x10;
		dsp_data[dsp_mode][0][offset+1] = 0x10;
		dsp_data[dsp_mode][0][offset+2] = 0x10;

		//second Line
		dsp_data[dsp_mode][1][offset] = 0x10;
		dsp_data[dsp_mode][1][offset+1] = 0x10; //custom char 1
		dsp_data[dsp_mode][1][offset+2] = 0x10; //custom char 1

		//third Line
		dsp_data[dsp_mode][2][offset] = 0x10;
		dsp_data[dsp_mode][2][offset+1] = 0x10;
		dsp_data[dsp_mode][2][offset+2] = 0x10;

		//fourth Line
		dsp_data[dsp_mode][3][offset] = 0x10;
		dsp_data[dsp_mode][3][offset+1] = 0x10;
		dsp_data[dsp_mode][3][offset+2] = 0x10;
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


void string_to_digit(uint8_t dsp_mode, char *string, uint8_t offset_column, uint8_t offset_line){

	for(size_t i = 0; i < strlen(string); i++){
			
	dsp_data [dsp_mode][offset_line][offset_column+i] = string[i];	
		}
}


//num_to_3digit this function not only converts the raw data to a 3Digit number which can be displayed onto the display, it also sets the Page and Position of the Number on the Display
void num_to_digit(uint8_t dsp_mode, uint16_t number, uint8_t comma, uint8_t digits, uint8_t offset_column, uint8_t offset_line){
		uint8_t digits_char[digits];
		uint8_t written_comma = 0;
		for(uint8_t i = 1; i <= digits; i++){
			uint8_t digit = number % 10; 
			digits_char[digits-i] = 0x30 + digit;
			number = number/10;
		}
		uint8_t first_digit_encountered = 0;
		for(uint8_t i = 0; i < digits; i++){
			
			if(digits_char[i] != 0x30){
				first_digit_encountered = 1;
			}
			if (!first_digit_encountered & i < (digits - comma -2)){
				digits_char[i] = 0x10;
			}
			
			if(i < digits-comma){
				dsp_data [dsp_mode][offset_line][offset_column+i] = digits_char[i];
			}else{
				if (!written_comma){
					dsp_data [dsp_mode][offset_line][offset_column+i] = '.';
					written_comma = 1;
				}
				dsp_data [dsp_mode][offset_line][offset_column+1+i] = digits_char[i];
			}
		}
		
	//}
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


void dsp_clear(void){
	//init the Display with and empty char
	for (int i = 0; i<4;i++){
		for (int s=0;s<20;s++){
			dsp_data[0][i][s]=0x10;
			dsp_data[1][i][s]=0x10;
			dsp_data[2][i][s]=0x10;
			dsp_data[3][i][s]=0x10;
		}
	}
}


void dsp_arrayinit_static(void)
{
//home screen
	//first argument is the display mode here i is always 0, second number is the row count (4 rows 1-3), 3argument is the Line count 0-20
	//Names
	dsp_data[0][0][0] = 'T';
	dsp_data[0][0][1] = 'S';
	dsp_data[0][0][2] = 'V';
	
	dsp_data[0][0][10] = 'S';
	dsp_data[0][0][11] = 'P';
	dsp_data[0][0][12] = 'D';
	
	dsp_data[0][1][0] = 'C';
	dsp_data[0][1][1] = 'O';
	dsp_data[0][1][2] = 'O';
	dsp_data[0][1][3] = 'L';
	
	dsp_data[0][1][10] = 'A';
	dsp_data[0][1][11] = 'C';
	dsp_data[0][1][12] = 'T';
	
	dsp_data[0][2][0] = 'B';
	dsp_data[0][2][1] = 'B';
	
	dsp_data[0][2][10] = 'S';
	dsp_data[0][2][11] = 'O';
	dsp_data[0][2][12] = 'C';
	
	dsp_data[0][3][10] = 'L';
	dsp_data[0][3][11] = 'V';
	dsp_data[0][3][12] = 'S';
	
	//Units
	dsp_data[0][0][8] = 'V';
	dsp_data[0][0][17] = 'K';
	dsp_data[0][0][18] = 'M';
	dsp_data[0][0][19] = 'H';
	
	dsp_data[0][1][8] = 'C';
	dsp_data[0][1][17] = 'C';
	
	dsp_data[0][2][8] = '-';
	dsp_data[0][2][17] = '%';
	
	dsp_data[0][3][17] = 'V';
	
//DEBUG INTERFACE
	//Names
	dsp_data[1][0][0] = 'C'; 
	dsp_data[1][0][1] = 'T';
	dsp_data[1][0][2] = 'M';
	dsp_data[1][0][3] = 'I';
	dsp_data[1][0][4] = 'N';
	
	dsp_data[1][0][13] = 'M';
	dsp_data[1][0][14] = 'A';
	dsp_data[1][0][15] = 'X';
	
	dsp_data[1][1][0] = 'C';
	dsp_data[1][1][1] = 'V';
	dsp_data[1][1][2] = 'M';
	dsp_data[1][1][3] = 'I';
	dsp_data[1][1][4] = 'N';
	
	dsp_data[1][1][13] = 'M';
	dsp_data[1][1][14] = 'A';
	dsp_data[1][1][15] = 'X';
	
	dsp_data[1][2][0] = 'A';
	dsp_data[1][2][1] = 'P';
	dsp_data[1][2][2] = 'P';
	dsp_data[1][2][3] = 'S';
	dsp_data[1][2][4] = '1';
	
	dsp_data[1][2][13] = '2';	
	
	dsp_data[1][3][0] = 'B';
	dsp_data[1][3][1] = 'P';
	dsp_data[1][3][2] = 'F';
	
	dsp_data[1][3][13] = 'R';
	
	//Units		
	dsp_data[1][0][10] = 'C';
	dsp_data[1][0][20] = 'C';

	dsp_data[1][1][10] = 'V';
	dsp_data[1][1][20] = 'V';
	
	dsp_data[1][2][10] = '%';
	dsp_data[1][2][20] = '%';	
	
	dsp_data[1][3][10] = 'B';
	dsp_data[1][3][20] = 'B';	
	
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