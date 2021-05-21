/*
 * display_functions.h
 *
 * Created: 03.05.2021 15:40:34
 *  Author: cerbe
 */ 
#include <avr/io.h>


#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_
#define RPM_MAX 9500
#define LED_LEFT  PORTA
#define LED_RIGHT PORTB
#define LED_Count 15
#define selftest_time 1200 //selftest_time in ms take times 2 for actual time

#define F_CPU 16000000

#define RPM_LED_PORT_1 PORTA
#define RPM_LED_PORT_2 PORTB

#define Blinkintervall 80 //Blinking time in ms per toggle so one on off cycle takes 2 the time
#define shiftrpm 8500

void dsp_writedata(uint8_t data,uint8_t rs);
void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);
void dsp_write(uint8_t dsp_mode);
void dsp_definechars();
void dsp_init();
void LED_RPM_Bar();
void RPM_LED_Blink();
void selftest();

#endif /* DISPLAY_FUNCTIONS_H_ */