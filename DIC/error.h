/*
 * error.h
 *
 * Created: 21.05.2021 11:08:08
 *  Author: lukas
 */ 
#include <avr/io.h>
#include <string.h>

#ifndef ERROR_H_
#define ERROR_H_

#define CLT_max 115 //°C
#define	CLT_min 60 //°C
#define OILP_min 10 //0.1Bar steps
#define Volt_max 150 //0.1V
#define Volt_min 120 //0.1V

void error_handling();
void add_error_codes(char *string1[], char *string2[]);


#endif /* ERROR_H_ */