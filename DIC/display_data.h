#ifndef DISPLAY_DATA_H_
#define DISPLAY_DATA_H_

//void large_number(uint8_t dsp_mode, uint8_t offset, uint8_t number);
//void num_to_digit(uint8_t dsp_mode, uint16_t number, uint8_t comma, uint8_t digits, uint8_t offset_column, uint8_t offset_line);
//void time_to_digit(uint8_t dsp_mode, uint32_t number, uint8_t offset_column, uint8_t offset_line);
//void dsp_arrayinit_static(void);
void dsp_clear();
//uint8_t calc_BB(uint8_t BPF, uint8_t BPR);//function to calculate the brake bias
void string_to_digit(uint8_t dsp_mode, char *string, uint8_t offset_column, uint8_t offset_line);

#endif