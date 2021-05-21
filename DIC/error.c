/*
 * error.c
 *
 * Created: 21.05.2021 11:07:58
 *  Author: lukas
 */ 
#include <avr/io.h>
#include "error.h"

extern uint8_t CLT;
extern uint8_t OILP;
extern uint8_t OILT;


//This functions checks for all possible error states, assignes a string or chracter to them and builds a string from them that an be shown in the error field on the home screen
char error_handling(){
	char Error_indicator[2] = "OK";
	char hello[4] = "HELLO";
	
	Error_indicator[strlen(Error_indicator)+strlen(hello)] = add2strings(Error_indicator, hello);
	/*
	if (CLT >= CLT_max)
	{
		Error_indicator += CLT_Error;
	}
	if (OILP <= OILP_min)
	{
		Error_indicator += OILP_Error;
	}
	else{
		Error_indicator = "OK";
	}
	*/
	return Error_indicator;
}

char add2strings(char string1[], char string2[]){
	char return_string[strlen(string1)+strlen(string2)];
	for(size_t i = 0; i < strlen(string1); i++){
		return_string[i] = string1[i];
	}
	for(size_t i = 0; i < strlen(string2); i++){
		return_string[i+strlen(string1)] = string2[i];
	}
	return return_string;
}
