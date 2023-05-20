#ifndef DISPLAY_FUNCTIONS_H_
#define DISPLAY_FUNCTIONS_H_

#define LED_COUNT_TOP_LEFT 5
#define LED_COUNT_TOP_RIGHT 10
#define selftest_time 1500 //selftest_time in ms takes times 2 for actual time
#define Blinkintervall 80
#define LED_Count 15
#define true 1

void dsp_writedata(uint8_t data,uint8_t rs);
void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);
void dsp_write(uint8_t dsp_mode);
void dsp_definechars();
void dsp_init();

void LED_Port_Blink();
void Write_LED_Control(uint16_t LED_Register);
uint16_t Read_LED_Control();
uint8_t invert_binary_number(uint8_t number_to_invert);

void clear_top_left_bar(void);
void clear_top_right_bar(void);
void led_top_left_bar(uint16_t max_value_l, uint16_t min_value_l, uint16_t current_value_l);
void led_top_right_bar(uint16_t max_value_r, uint16_t min_value_r, uint16_t current_value_r);
void led_left_top_bar_select(uint8_t select_l);
void led_right_top_bar_select(uint8_t select_r);

void selftest();
//void shutdownLED();

#endif