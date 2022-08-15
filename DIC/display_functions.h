#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_

#define LED_COUNT_Top_LEFT 5
#define LED_COUNT_Top_RIGHT 10
#define selftest_time 1500 //selftest_time in ms take times 2 for actual time

//#define F_CPU 16000000

void dsp_writedata(uint8_t data,uint8_t rs);
//void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);
void dsp_write(uint8_t dsp_mode);
//void dsp_definechars();
void dsp_init();
void _Clear_Top_Left_Bar_(void);
void _Clear_Top_Right_Bar_(void);
void _LED_Top_Left_Bar_(uint16_t max_value_l, uint16_t min_value_l, uint16_t current_value_l);
void _LED_Top_Right_Bar_(uint16_t max_value_r, uint16_t min_value_r, uint16_t current_value_r);
void _LED_Left_Top_Bar_Select_(uint8_t select_l);
void _LED_Right_Top_Bar_Select_(uint8_t select_r);
void selftest();
//void shutdownLED();

#endif