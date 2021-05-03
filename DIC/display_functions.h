/*
 * display_functions.h
 *
 * Created: 03.05.2021 15:40:34
 *  Author: cerbe
 */ 


#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_


void dsp_writedata(uint8_t data,uint8_t rs);
void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);
void dsp_write();
void dsp_definechars();
void dsp_initdsp();



#endif /* DISPLAY_FUNCTIONS_H_ */