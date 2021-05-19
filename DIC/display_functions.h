/*
 * display_functions.h
 *
 * Created: 03.05.2021 15:40:34
 *  Author: cerbe
 */ 
#include <avr/io.h>


#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_
#define RPM_MAX 10000
#define LED_LEFT  PORTA
#define LED_RIGHT PORTB

#define max_RPM 10000
#define LED_Count 15

#define RPM_LED_PORT_1 PORTA
#define RPM_LED_PORT_2 PORTB

void dsp_writedata(uint8_t data,uint8_t rs);
void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);
void dsp_write(uint8_t dsp_mode);
void dsp_definechars();
void dsp_init();
void LED_RPM_Bar();

#endif /* DISPLAY_FUNCTIONS_H_ */