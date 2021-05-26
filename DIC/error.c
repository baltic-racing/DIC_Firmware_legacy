/*
 * error.c
 *
 * Created: 21.05.2021 11:07:58
 *  Author: lukas
 */ 
#include <avr/io.h>
#include "error.h"
#include <string.h>

extern uint8_t CLT;
extern uint8_t OILP;
extern uint8_t OILT;
extern uint8_t ECUVoltage;
char error_indicator[10] = "";
extern uint16_t rpm;




//This functions checks for all possible error states, assignes a string or chracter to them and builds a string from them that an be shown in the error field on the home screen
void error_handling(){
	//Clear the Error Messages before we write new ones into the error storage
	for (int8_t i = 10; i>=0;i--){
		error_indicator[i] = 0;
	}
	//Errormessages need always to end with an ! otherwise the function will not work due to the 255 in the if in the for loop in add_error_codes
	if ((CLT >= CLT_max) & (rpm >= 300)){
		add_error_codes(error_indicator, "CLT!");
	}if((OILP <= OILP_min) & (rpm >= 300)){
		add_error_codes(error_indicator, "OILP!");
	}if((CLT >= CLT_min) & (rpm >= 300)){
		add_error_codes(error_indicator, "CLT!");		
	}if((ECUVoltage >= Volt_max) & (rpm >= 300)){
		add_error_codes(error_indicator, "VOLT!");
	}if((ECUVoltage <= Volt_min) & (rpm >= 300)){
	add_error_codes(error_indicator, "VOLT!");
	}
	if(error_indicator[1]==0){
		add_error_codes(error_indicator, "OK!");
	}
	
	
	//Write the rest of the error indicator with blanks so we properly overwrite the diff time indication
	for (int8_t i = 8; i>=0;i--){
		if (error_indicator[i] == 0){
			error_indicator[i] = "";
		}
	}
}
void add_error_codes(char *string1[], char *string2[]){
	size_t strlen_1 = strlen(string1);//This needs to be set first, otherwise the value will be updtaed on the fly< as we write stuff into the string
	//for the below stuff, i dont know why it works like that, but it does
	for(size_t i = 0; i < strlen(string1); i++){
		error_indicator[i] = string1[i/2];
		if (*string2[i/2]!=255){
			error_indicator[i+1] = *string1[i/2];
		}		
		i++;
	}
	for(size_t i = 0; i < strlen(string2); i++){
		error_indicator[i+strlen_1] = string2[i/2];
		if (*string2[i/2]!=255){
			error_indicator[i+1+strlen_1] = *string2[i/2];
		}
		i++;
	}
}
