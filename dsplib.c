//
// Created by sbiewald on 17.11.18.
//

#include <avr/cpufunc.h>
#include <avr/io.h>

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


void dsp_init() {
    for (int i=0; i<7;i++){
        dsp_write_raw(dsp_command[i],0);
    }
}


void dsp_clear_array(uint8_t screen[4][20]) {
    /*
     * Clear an array for display.
     */
    for (char row=0; row<4; row++) {
        for (char column=0; column<20; column++) {
            screen[row][column] = 0x10;
        }
    }
};


void dsp_write(uint8_t screen[4][20]) {
    /*
     * Writes the screen content, one character per call.
     *
     * The global variables dsp_row and dsp_column are used.
     */
    if (dsp_column<20) {
        dsp_write_raw(screen[dsp_row][dsp_column++], 1);
    }

    if (dsp_column >= 20) { // go to next row
        // reset column counter
        dsp_column = 0;

        dsp_write_raw( // write first charecter of next line
                screen[dsp_row++][dsp_column]
        );
    }

    if (dsp_linecounter == 4){ // prevent overflow
        dsp_row = 0;
    }
}


void dsp_write_raw(uint8_t data, uint8_t ctrl) {
    /*
     * Write raw data to the display and set the control pins.
     *
     * PINS are controlled by the DSP_PORT_DATA and DSP_PORT_CTRL defines.
     */

    // Write data
    DSP_PORT_DATA = data;

    // Set R/S pin
    if (ctrl==1) {
        DSP_PORT_CTRL |= (1 << DSP_PIN_RS);
    } else {
        DSP_PORT_CTRL &= ~(1 << DSP_PIN_RS);
    }


    // Set enable to high
    DSP_PORT_CTRL |= (1<<DSP_PIN_ENABLE);

    // Set enable to low after a delay
    // Discuss: Use proper delay function?
    for (char wait = 0; wait<100; wait++){
        // char instead of int saves memory
        _NOP();  // Never optimize away. 50 might be better.
    }

    // Read and process data pins
    DSP_PORT_CTRL &= ~(1<<DSP_PIN_ENABLE);
}


//void dsp_custom_char(uint8_t charpos, )




// void dsp_gearindicator()



void dsp_customchar(uint8_t charpos, uint8_t line1, uint8_t line2, uint8_t line3, uint8_t line4, uint8_t line5, uint8_t line6, uint8_t line7, uint8_t line8)
{
    /*
     * TODO: Evaluate this function and improve signature.
     *
     * */
    //where to write in cgram
    dsp_writedata(0x40+8*charpos,0);

    //write each line
    dsp_writedata(line1,1);
    dsp_writedata(line2,1);
    dsp_writedata(line3,1);
    dsp_writedata(line4,1);
    dsp_writedata(line5,1);
    dsp_writedata(line6,1);
    dsp_writedata(line7,1);
    dsp_writedata(line8,1);

    //dsiplay return home
    dsp_writedata(0x02,0);
}


