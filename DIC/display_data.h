/*
 * display_data.h
 *
 * Created: 03.05.2021 15:55:46
 *  Author: Ole H.
 */ 
#include <stdint.h>
#include <avr/io.h>

#ifndef DISPLAY_DATA_H_
#define DISPLAY_DATA_H_

void large_number(uint8_t dsp_mode, uint8_t offset, uint8_t number);
void num_to_3digit(uint8_t dsp_mode, uint16_t number, uint8_t comma, uint8_t digits);
void dsp_arrayinit(void);

#endif /* DISPLAY_DATA_H_ */