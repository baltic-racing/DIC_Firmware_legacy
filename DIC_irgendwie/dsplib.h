//
// Created by sbiewald on 17.11.18.
//

#ifndef TY19_DIC_DSPLIB_H
#define TY19_DIC_DSPLIB_H

void dsp_init();
//
// Created by sbiewald on 17.11.18.
//

#include <avr/cpufunc.h>
#include <avr/io.h>

#include "canlib.h"

#include "dsplib.h"

#ifndef DSP_PORT_DATA
    #define DSP_PORT_DATA   PORTE
#endif
#ifndef DSP_PORT_CTRL_SET
    #define DSP_PORT_CTRL   PORTC
    #define DSP_PIN_RS PC0
    #define DSP_PIN_RW PC1
    #define DSP_PIN_ENABLE  PC2
#endif

/* It is unknown where this came from. */
uint8_t DSP_LINE[4] ={
        0xC0, // Line 4
        0x94, // Line 2
        0xD4, // Line 3
        0x80, // Line 1
};

/* Initialize display */
uint8_t DSP_INIT_COMMAND[7] = {
        0x39, // function set european character set
        0x08, // display off
        0x06, // entry mode set increment cursor by 1 not shifting display
        0x17, // Character mode and internal power on
        0x00, // clear display
        // 0x01, // NEVER USE CLS IT WILL GET THE DISPLAY HOT SINCE EXECUTION TIME >2 ms
        0x02, // return home
        0x0C, // display on
};

/* Current row or column of the display */
uint8_t dsp_row=0;
uint8_t dsp_column=0;


void dsp_init();


void dsp_clear_array(uint8_t screen[4][20]);

void dsp_write(uint8_t screen[4][20]);


void dsp_write_raw(uint8_t data, uint8_t ctrl);


void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8);





#endif //TY19_DIC_DSPLIB_H
