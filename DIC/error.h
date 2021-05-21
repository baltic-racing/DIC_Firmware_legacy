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

#define CLT_max 110 //°C
#define OILP_min 10 //0.1Bar steps

void error_handling();
void add_error_codes(char *string1[], char *string2[]);


#endif /* ERROR_H_ */