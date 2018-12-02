//
// Created by sbiewald on 17.11.18.
//

#include <avr/interrupt.h>
#include <avr/io.h>
#include <string.h>

#include "canlib.c"

// Array in which Data is saved which will be drawn onto the Display
uint8_t dsp_data [2][4][20];
uint8_t dsp_mde = 1;


ISR(TIMER1_COMPA_vect) {
    draw_data = 1;
    update_data++;
}


void timer_config(void) {
    /*
     * Configure the timers.
     *
     * TODO: Find out what the heck this does.
     */
    //TIMER CONFIG
    TCCR1B |= (1<<CS12) | (1<<CS10) | (1<<WGM12);
    TIMSK1 |= (1<<OCIE1A);
    //start with a logical high time
    OCR1A = 15;
}


void port_init() {
    DDRE = 0xFF;
    DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
    PORTC = 0;
    PORTE = 0;
}


void dsp_definechars() {
    /*
     * Setup custom character required for
     */
    dsp_customchar(0,0,0,0,0,0,0x1F,0x1F,0x1F);
    dsp_customchar(1,0x1F,0x1F,0,0,0,0,0,0);
    dsp_customchar(2,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF);
    dsp_customchar(3,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E);
    dsp_customchar(4,0xFF,0xFF,0xFF,0xFF,0,0,0,0);
    dsp_customchar(5,0xFF,0xFF,0xFF,0xFF,0xEF,0xEF,0xEF,0xEF);
    dsp_customchar(6,0x08,0x15,0x0A,0x04,0x0A,0x15,0x02,0x00);

}


void dsp_setup_debug(uint8_t screen[4][20]) {
    /*
     * Setup the debug display.
     *
     * TODO: Reformat and shorten.
     */
    screen[0][0] = 'B'; screen[0][1] = 'P'; screen[0][2] = 'F'; screen[0][3] = 'R'; screen[0][4] = '='; screen[0][7] = 'B';	//implemented
    screen[1][0] = 'B'; screen[1][1] = 'P'; screen[1][2] = 'R'; screen[1][3] = 'E'; screen[1][4] = '='; screen[1][7] = 'B';	//implemented
    screen[2][0] = 'B'; screen[2][1] = 'T'; screen[2][2] = 'L'; screen[2][3] = '='; screen[2][7] = 'C';							//implemented
    screen[3][0] = 'B'; screen[3][1] = 'T'; screen[3][2] = 'R'; screen[3][3] = '='; screen[3][7] = 'C';							//implemented

    screen[0][9] = 'V'; screen[0][10] = 'B'; screen[0][11] = '='; screen[0][14] = '.'; screen[0][16] = 'V';						//implemented
    screen[1][9] = 'T'; screen[1][10] = 'P'; screen[1][11] = 'S'; screen[1][12] = '='; screen[1][16] = 0x06; 					//implemented
    screen[2][9] = 'O'; screen[2][10] = 'i'; screen[2][11] = 'T'; screen[2][12] = '='; screen[2][16] = 'C';            //implemented
    screen[3][9] = 'S'; screen[3][10] = 'l'; screen[3][11] = 'i'; screen[3][12] = '='; screen[3][16] = 0x06;
}


void dsp_setup_production(uint8_t screen[4][20]) {
    /*
     * Setup the production display.
     */

    // Cooling temperature
    memcpy(&screen[0][0], (uint8_t *)"COOL.");  // Not sure if the cast is necessary
    screen[0][14] = 'C';

    // Oil pressure and temperature
    memcpy(&screen[1][0], (uint8_t *)"OEL");
    screen[1][8] = 'B';
    screen[1][14] = 'C';

    memcpy(&screen[3][0], (uint8_t *)"K=");
    memcpy(&screen[3][0], (uint8_t *)"E=");
}


void refresh_gear(uint8_t gear, uint8_t screen[4][20]) {
    /*
     * Refresh the (big) gear indicator on the right of screen.
     * Custom chars are used, see dsp_definechars();
     * The functions is a bit long - optimizations welcome!
     *
     * Only useful on the production display.
     */

    switch (gear){

        //NEUTRAL
        case 0:
            //First Line
            screen[0][17] = 0xFF;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            //second Line
            screen[1][17] = 0xFF;
            screen[1][18] = 0x10;
            screen[1][19] = 0xFF;

            //third Line
            screen[2][17] = 0xFF;
            screen[2][18] = 0x10;
            screen[2][19] = 0xFF;

            //fourth Line
            screen[3][17] = 0xFF;
            screen[3][18] = 0xFF;
            screen[3][19] = 0xFF;
            break;

            //First Gear
        case 1:
            //First Line
            screen[0][17] = 0x10;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            //second Line
            screen[1][17] = 0x10;
            screen[1][18] = 0x10;
            screen[1][19] = 0xFF;

            //third Line
            screen[2][17] = 0x10;
            screen[2][18] = 0x10;
            screen[2][19] = 0xFF;

            //fourth Line
            screen[3][17] = 0x10;
            screen[3][18] = 0x10;
            screen[3][19] = 0xFF;
            break;

            //Second Gear
        case 2:
            //First Line
            screen[0][17] = 0xFF;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            //second Line
            screen[1][17] = 0x00; //custom char 1
            screen[1][18] = 0x00; //custom char 1
            screen[1][19] = 0xFF;

            //third Line
            screen[2][17] = 0xFF;
            screen[2][18] = 0x01; //custom char 2
            screen[2][19] = 0x01; //custom char 2

            //fourth Line
            screen[3][17] = 0xFF;
            screen[3][18] = 0xFF;
            screen[3][19] = 0xFF;
            break;

            //Third Gear
        case 3:
            //First Line
            screen[0][17] = 0xFF;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            //second Line
            screen[1][17] = 0x00; //custom char 1
            screen[1][18] = 0x00; //custom char 1
            screen[1][19] = 0xFF;

            //third Line
            screen[2][17] = 0x01; //custom char 2
            screen[2][18] = 0x01; //custom char 2
            screen[2][19] = 0xFF;

            //fourth Line
            screen[3][17] = 0xFF;
            screen[3][18] = 0xFF;
            screen[3][19] = 0xFF;
            break;

            //Fourth Gear
        case 4:
            //First Line
            screen[0][17] = 0x03; //custom char 4
            screen[0][18] = 0x10;
            screen[0][19] = 0x10;

            //second Line
            screen[1][17] = 0x03; //custom char 4
            screen[1][18] = 0x02; //custom char 3
            screen[1][19] = 0x10;

            //third Line
            screen[2][17] = 0x04; //custom char 5
            screen[2][18] = 0x05; //custom char 6
            screen[2][19] = 0x04; //custom char 5

            //fourth Line
            screen[3][17] = 0x10;
            screen[3][18] = 0x02; //custom char 3
            screen[3][19] = 0x10;
            break;

            //Fith Gear
        case 5:
            //First Line
            screen[0][17] = 0xFF;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            //second Line
            screen[1][17] = 0xFF;
            screen[1][18] = 0x00; //custom char 1
            screen[1][19] = 0x00; //custom char 1

            //third Line
            screen[2][17] = 0x01; //custom char 2
            screen[2][18] = 0x01; //custom char 2
            screen[2][19] = 0xFF;

            //fourth Line
            screen[3][17] = 0xFF;
            screen[3][18] = 0xFF;
            screen[3][19] = 0xFF;
            break;

            //6th Gear
        case 6:
            //First Line
            screen[0][17] = 0xFF;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            //second Line
            screen[1][17] = 0xFF;
            screen[1][18] = 0x00; //custom char 1
            screen[1][19] = 0x00; //custom char 1

            //third Line
            screen[2][17] = 0xFF;
            screen[2][18] = 0x10;
            screen[2][19] = 0xFF;

            //fourth Line
            screen[3][17] = 0xFF;
            screen[3][18] = 0xFF;
            screen[3][19] = 0xFF;
            break;

            //if no valid gear was transmitted
        default:
            screen[0][17] = 0xFF;
            screen[0][18] = 0xFF;
            screen[0][19] = 0xFF;

            screen[1][17] = 0xFF;
            screen[1][18] = 0xFF;
            screen[1][19] = 0xFF;

            screen[2][17] = 0xFF;
            screen[2][18] = 0xFF;
            screen[2][19] = 0xFF;

            screen[3][17] = 0xFF;
            screen[3][18] = 0xFF;
            screen[3][19] = 0xFF;
            break;
    }
}


void refresh_battery(
        uint8_t *battery_raw,
        uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh the battery on the screen.
     * Output precision is 100mV.
     *
     * battery_raw are the two battery bytes.
     *
     * Output width is 4.
     *
     * Currently used only on the debugging screen.
     */

    uint8_t v_bat = (battery_raw[0] + (battery_raw[1] << 8)) * 0.27;
    uint8_t v_bat_001 = v_bat % 10;
    uint8_t v_bat_100 = v_bat / 100;
    uint8_t v_bat_010 = (v_bat-v_bat_001-v_bat_100*100) / 10;

    if (v_bat_100 == 0) {
        screen[row][column] = 0x10;
    } else {
        screen[row][column] = 0x30 + v_bat_100;
    }
    screen[row][column+1] = v_bat_010;
    // screen[row][column+2] = '.';
    screen[row][column+3] = v_bat_001;
}


void refresh_oil_temperature(
        uint8_t oil_temperature,
        uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh the cooling temperature on the screen.
     * oil_raw the direct oil temperature.
     *
     * Output width is 3
     *
     * Currently used on production screen.
     */
    uint8_t oil_temperature_100 = oil_temperature / 100;
    uint8_t oil_temperature_001 = oil_temperature % 10;
    uint8_t oil_temperature_010 = (
            oil_temperature - oil_temperature_001 - oil_temperature_100 * 100) / 10;

    if (oil_temperature_100 == 0) {
        screen[row][column] = 0x10;
    } else {
        screen[row][column] = 0x30 + oil_temperature_100;
    }
    if (oil_temperature_100 == 0 && oil_temperature_010 == 0) {
        screen[row][column+1] = 0x10;
    } else {
        screen[row][column+1] = 0x30 + oil_temperature_010;
    }
    screen[row][column+1] = v_bat_001;
}


void refresh_oil_pressure(
    uint8_t oil_pressure_raw,
    uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh the oil pressure.
     *
     * oil_pressure_raw = oil_pressure / 6.25
     * Output width 3.
     */
    uint16_t oil_pressure = oil_pressure_raw * 6.25;
    uint8_t oil_pressure_100 = oil_pressure / 100;
    uint8_t oil_pressure_001 = oil_pressure % 10;
    uint8_t oil_pressure_010 = (
        oil_pressure - oil_pressure_001 - oil_pressure_100 * 100) / 10;

    screen[row][column] = 0x30 + oil_pressure_100;
    // screen[row][column+1] = '.';
    screen[row][column+2] = 0x30 + oil_pressure_010;
    // screen[row][column+3] = 0x30 + oil_pressure_001;
}



void refresh_cooler_temperature(
        uint8_t *cooler_temperature_raw,
        uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh the cooler temperature on screen.
     *
     * cooler_temperature are in two bytes.
     *
     * Output width is 3.
     *
     * Currently used on the production screen.
     */

    int16_t cooler_temperature = cooler_temperature_raw[0] | (cooler_temperature_raw[1] << 8);

    if (cooler_temperature < 0) cooler_temperature = 0;  // it cannot freeze

    cooler_temperature_100 = cooler_temperature / 100;
    cooler_temperature_001 = cooler_temperature % 10;
    cooler_temperature_010 = (
            cooler_temperature - cooler_temperature_001 - cooler_temperature_100 * 100) / 10;

    if (cooler_temperature_100 == 0) {
        screen[row][column] = 0x10;
    } else {
        screen[row][column] = 0x30 + cooler_temperature_100;
    }
    if (cooler_temperature_100 == 0 && cooler_temperature_010 == 0) {
        screen[row][column+1] = 0x10;
    } else {
        screen[row][column+1] = 0x30 + cooler_temperature_010;
    }
    screen[row][column+2] = 0x30 + cooler_temperature_001;
}


void refresh_rpm(uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh the revolutions per minute counter on the screen.
     *
     * Currently not used at all. It is not implemented.
     */
}


void refresh_break_pressures(uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh break pressure on screen.
     *
     * Currently only used on the debugging screen.
     */
}


void refresh_break_temperatures(uint8_t screen[4][20], int row, int column) {
    /*
     * Refresh break temperature on screen.
     *
     * Currently only used on the debugging screen.
     */
}


void init() {
    /*
     * Initializes display, can, ports, ...
     */
    can_init();
    port_init();

    dsp_init();
    dsp_definechars();
}


int main() {
    uint8_t * screen[20];
    uint8_t screen_production [4][20];
    uint8_t screen_debug[4][20];

    screen = &screen_production;

    init();

    dsp_setup_production(screen_production);
    dsp_setup_debug(screen_debug);

    CAN_MOB_ID can_data[7] = {
            {0, CAN_ID_SWC},
            {1, CAN_ID_CMC},
            {2, CAN_ID_DATALOGGER_VOLTAGE},
            {3, CAN_ID_DATALOGGER_BREAKPRESSURE},
            {4, CAN_ID_ECU1},
            {5, CAN_ID_ECU2},
            {6, CAN_ID_ECU4}
    };

    char row=0;
    char column=0;

    while (1) {
        if (counter_update_data >= 44) {
            counter_update_data = 0;

            can_multi_rx(can_data);

            // change display mode between 'production' and debug
            dsp_mde = can_data_bytes[1][4] & 0b00001000;

            if (dsp_mde>1) dsp_mde=1;

            if (dsp_mde == 0) {  // production
                screen = &screen_production;
                // First byte of SWC data is the gear
                refresh_gear(CAN_DATA_BYTES[1][0], screen_production);

                // Refresh other status information.
                refresh_oil_pressure(CAN_DATA_BYTES[4][4], screen_production, 1, 4);
                refresh_oil_temperature(CAN_DATA_BYTES[4][3], screen_production, 1, 10);
                refresh_cooler_temperature(CAN_DATA_BYTES[4]+6, screen_production, 0, 10);

            } else {  // debug
                screen = &screen_debug;
                // Battery bytes are at
                // [6][0] and [6][1]
                refresh_battery(CAN_DATA_BYTES[6]+1, screen_debug, 0, 12);
                refresh_oil_temperature(CAN_DATA_BYTES[4][3], screen_debug, 2, 13);
            }

        }

        dsp_write(*screen);
    }

}