/*
 * error.c
 *
 * Created: 21.05.2021 11:07:58
 *  Author: lukas
 */ 
#include <avr/io.h>
#include "error.h"
#include <string.h>

extern uint16_t CLT;
extern uint16_t OILP;
extern uint8_t OILT;
extern uint16_t ECUVoltage;
char error_indicator[10] = "";
extern uint16_t rpm;




//This functions checks for all possible error states, assignes a string or chracter to them and builds a string from them that an be shown in the error field on the home screen
void error_handling(){
	//Error messages need always to end with an ! otherwise the function will not work due to the 255 in the if in the for loop in add_error_codes
	uint8_t written_msg = 0;
	
	string_to_digit(0,"          ",0,3); //clear the error message space
	
	if ((CLT >= CLT_max) & (rpm >= 300))
	{
		string_to_digit(0,"CLT!",written_msg,3);
		written_msg+=strlen("CLT!");
	}
	if((OILP <= OILP_min) & (rpm >= 300 & written_msg + strlen("OILP!") <= 10))
	{
	string_to_digit(0,"OILP!",written_msg,3);
	written_msg+=strlen("OILP!");	
	}
	if((CLT <= CLT_min) & (rpm >= 300) & written_msg + strlen("CLT!") <= 10)
	{
	string_to_digit(0,"CLT!",written_msg,3);
	written_msg+=strlen("CLT!");	
	}
	if((ECUVoltage >= Volt_max) & written_msg + strlen("VOLT!") <= 10)
	{
	string_to_digit(0,"VOLT!",written_msg,3);
	written_msg+=strlen("VOLT!");
	}
	if((ECUVoltage <= Volt_min) & written_msg + strlen("VOLT!") <= 10)
	{
		string_to_digit(0,"VOLT!",written_msg,3);
		written_msg+=strlen("VOLT!");
	}
	if(written_msg == 0)
	{
		string_to_digit(0,"OK",0,3);
	}
}