//
// Created by sbiewald on 17.11.18.
//

#include <avr/interrupt.h>
#include <avr/io.h>

#define F_CPU 1600000UL  // 1 MHz
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "messages.h"

#include "canlib.h"
#include "display_functions.h"
#include "safety_limits.h"

#define TEST_BLINK

#define DSP_MDE_TEST 2
#define DSP_MDE_PROD 1
#define DSP_MDE_DEBUG 0

// Array in which Data is saved which will be drawn onto the Display

uint8_t dsp_data [3][4][21];  // 20 chars, 21 is null terminated
uint8_t dsp_mde = DSP_MDE_PROD;

#define DSP_TEST dsp_data[DSP_MDE_TEST]
#define DSP_PROD dsp_data[DSP_MDE_PROD]
#define DSP_DEBUG dsp_data[DSP_MDE_DEBUG]

int line = 0;

unsigned long touch=0;

// While CMC and ECU(4) can transfer gear information, 

// Can mobs
struct CAN_MOB can_swc = {0x100, 0xfff, 0};		// SWC feedback
struct CAN_MOB can_cmc = {0x200, 0xfff, 1};		// CMC, Gear
struct CAN_MOB can_datalogger_v;
struct CAN_MOB can_datalogger_pressure;
struct CAN_MOB can_ecu_0 = {0x600, 0xfff, 2};	// ECU(1): RPM, Tps, Map, iat, injpw
struct CAN_MOB can_ecu_2 = {0x602, 0xfff, 3};	// ECU(2): Vspd, baro, oil temperatur, fuelp, clt
struct CAN_MOB can_voltage = {0x604, 0xfff, 4};	// ECU(4): gear, voltage, ecu temperature, error
struct CAN_MOB can_fuse = {0x300, 0xfff, 7};    // Fuse

#define LED_LEFT_CONF DDRA
#define LED_RIGHT_CONF DDRB

#define LED_LEFT  PORTA
#define LED_RIGHT PORTB


// Declarations
void setup();
void setup_dsp_empty(uint8_t screen[4][21]);
void setup_dsp_debug(uint8_t screen[4][21]);
void setup_dsp_production(uint8_t screen[4][21]);
void setup_dsp_customchars();

void refresh_display_with_can_data(int mode, uint8_t screens[][4][21]); 

void refresh_battery(uint8_t battery_raw[2], uint8_t screen[4][21], int row, int column);
void refresh_gear(uint8_t gear, uint8_t screen[4][21]);
void refresh_rpm(uint16_t rpm);
void refresh_rpm_raw_can(uint8_t rpm_raw[2]);

void refresh_cooler_temperature(uint8_t cooler_temperature_raw[2], uint8_t screen[4][21], int row, int column);
void refresh_oil_pressure(uint8_t oil_pressure_raw, uint8_t screen[4][21], int row, int column);
void refresh_oil_temperature(uint8_t oil_temperature, uint8_t screen[4][21], int row, int column);
int write_dsp(uint8_t screen[4][21]);
void selftest();

#define RPM_MAX 10000
#define BLINK_SLOW ((status.systime & 2048) == 2048)
#define BLINK_FAST ((status.systime & 1024) == 1024)

uint16_t rpm_ranges[12];

// Global error messages

struct {
	int error;
	char * current_message;
	unsigned long systime;
	struct MessageList error_messages;
	struct {
		bool voltage;
		bool break_temperature;
		bool oil_pressure;
		bool cooling_temperature;
	} errors;
	uint16_t rpm;
	bool car_running;
	bool low_oil_pressure;
	bool low_oil_pressure_error;
#ifdef HIGH_OIL_PRESURE_CHECK
	bool high_oil_pressure;
#endif
} status = {false, NULL, 0, {NULL} , {false, false, false, false}, 0, false};

void calculate_rpm_ranges() {
	uint16_t rpm_real_size = sizeof(rpm_ranges) / 2; 
	for (uint16_t i=1; i<rpm_real_size+1; i++){
		rpm_ranges[i-1]=RPM_MAX/rpm_real_size*i;
	}
};


int main(void){
	uint8_t swc[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	setup();
	selftest();

	//setup_dsp_empty(dsp_data[dsp_mde]);

	while (1) {
		can_rx(&can_swc, swc);
		
		dsp_mde = ~swc[0] & 1;

		refresh_display_with_can_data(dsp_mde, dsp_data);
		write_dsp(dsp_data[dsp_mde]);
		
		_delay_ms(2);
	
	}
}


ISR(TIMER0_COMP_vect) {
	/* Interrupt actions for Timer0: Increase time */
	static signed long oil_pressure_timer = 4096*OIL_PRESSURE_LOW_TIME;
	
	cli();
	if (status.low_oil_pressure && oil_pressure_timer >= 1) {
		oil_pressure_timer--;
	} else {
		oil_pressure_timer = 4096*OIL_PRESSURE_LOW_TIME;
	}
	
	if (oil_pressure_timer <= 0) {
		status.low_oil_pressure_error |= true;
	}
	sei();
	
	status.systime++;
}

ISR(TIMER1_COMPA_vect) {
	// "Slow" UI timer
	
	if (status.error) {
		LED_RIGHT |= 0b1000000;
		status.current_message = get_next_message(&status.error_messages);
	}
	
	// Call "fast" function
	// isr_fast_ui();
}

ISR(TIMER3_COMPA_vect) {
	// "Fast" UI timer
	isr_fast_ui();
}

inline void isr_fast_ui() {
	// Inline to avoid jump overhead
	/*if (status.error) {
		LED_LEFT  = rand();
		LED_RIGHT = (LED_RIGHT & 0x40) | rand();
	}*/
}

void check_safety_limits() {}
void update_safety_critical_values() {}

ISR(TIMER1_COMPC_vect) {
	// "Slow" UI timer
	status.current_message = get_next_message(&status.error_messages);
	// dsp_data[0][0][0] += 1;
}

void setup_timer(void) {
    /*
     * Configure the timers.
	 * Timer 0, CTC, every 1024*2 = 2048 cycles
     * Timer 1, CTC, every 1024*2 = 2048 cycles
     */
	
	//TIMER0 CONFIG (systime)
	TCCR0A |= (1<<WGM01); // CTC mode
	TCCR0A |= (1<<CS02) | (1<<CS00);  // 1024
	
	OCR0A = 1; // 2
	TIMSK0 = (1<<OCIE0A); // Interrupt on compare
	
    //TIMER1 CONFIG (Slow UI functions)
    TCCR1B |= (1<<CS12) | (1<<CS10);	// Set pre-scaler to one tick every 1024 clock cycles
	TCCR1B |= (1<<WGM12);				// Interrupt mode CTC
	
	OCR1A = 4096*2-1;  // 1.00 second
	TIMSK1 |= (1<<OCIE1A);  // Use Interrupt register A
	
	//TIMER3 CONFIG (Fast UI functions)
	TCCR3B |= (1<<CS32) | (1<<CS30);	// Set pre-scaler to one tick every 1024 clock cycles
	TCCR3B |= (1<<WGM32);				// Interrupt mode CTC
	OCR3A = 2048-1;    // 0.20 seconds
	
	TIMSK3 |= (1<<OCIE3A);
}

void setup() {
	/*
     * Setup & configuration
     */
	
	//status.systime = 0;
	//status.error = false;
	//status.current_message = NULL;
	
	calculate_rpm_ranges();
	
	LED_LEFT_CONF = 0xff;	// Left LEDs as output
	
	LED_RIGHT_CONF = 0xff;	// Right LEDs as output
	

	// Setup display
	Display_Read_Disable();
	DDRC = (1<<PC0 | 1<<PC1 | 1<<PC2); // TODO: -> to Display_Setup
	Display_Setup();

	setup_dsp_customchars();
	
	// Clear display memory
	int i;
	for (i=0; i<3; i++) {
		setup_dsp_empty(dsp_data[i]);
	}
	
	setup_dsp_debug(dsp_data[0]);
	setup_dsp_production(dsp_data[1]);
	
	// Setup timer
	setup_timer();

	// CAN
	can_cfg();
	
	// Just that rand() is working
	//   Every restart, rand() will return the same numbers
	srand(0);
	
	sei();		// Enable interrupts
}

void showVersionsInformation() {

	Display_Goto_Position(0, 0);
	Display_Write_String("      DIC TY20      ");
	Display_Goto_Position(1, 0);
	Display_Write_String("       V 1.0.0      ");
	Display_Goto_Position(2, 0);
	Display_Write_String("Proudly presented by");
	Display_Goto_Position(3, 0);
	Display_Write_String(" dept. Electronics  ");

}
void selftest() {
	/*
	 * Run selftest:
     *  - LEDs
     *  - Display
     */
	LED_LEFT = 0xff;	// Test left LEDs
	LED_RIGHT = 0xff;	// Test right LEDs

	Display_Clear();

	showVersionsInformation();

	_delay_ms(3000);

	Display_Clear();
	Display_Goto_Position(0, 0);
	Display_Write_String("Baltic Racing - TY20");
	Display_Goto_Position(1, 0);
	Display_Write_String("   Engineered for   ");
	Display_Goto_Position(2, 0);
	Display_Write_String("   success!         ");
	Display_Goto_Position(3, 0);
	Display_Write_String("NOT BURNING TODAY! ");

	_delay_ms(100);
	
	cli();
	
	// Test refresh_rpm.
	// The is reset as it will otherwise flash the LEDs.
	//
	// For this reason following is happening:
	//  - Disable global interrupts with cli()
	//  - Save the systime
	//  - Set global systime to 0
	//  [test refresh_rpm()]
	//  - Restore systime
	//  - Enable interrupts with sei()
	
	unsigned long int old_systime_val = status.systime;
	status.systime = 0;
	
	signed int i;
	uint16_t rpm;
	for (i=sizeof(rpm_ranges)/2-1; i > -1; i--) {
		//uint16_t rpm = rpm_ranges[i] + RPM_MAX / (sizeof(rpm_ranges)/2);
		rpm = rpm_ranges[i]+1;
		refresh_rpm(rpm);
		
		_delay_ms(150);
	}
	
	refresh_rpm(rpm-1);
	_delay_ms(250);
	
	status.systime = old_systime_val;
	
	sei();
	

	// Reset
	
	PORTA = 0;
	PORTB = 0;

	Display_Clear();
	Display_Goto_Position(0, 0);
	volatile uint8_t test[8]={0,0,0,0,0,0,0,0};
	can_rx(&can_voltage, test);		// First time, no result
	_delay_ms(250);
	can_rx(&can_voltage, test);		// Actual results, here
	uint8_t volatile voltage = (test[3]<<8 | test[2])*0.27;

	if (!voltage) {
		Display_Write_String("CAN: ERROR!");
	} else {
		Display_Write_String("CAN: OK.");
	}
	
	for (int j=0; i<12; i++) {
		refresh_rpm(RPM_MAX);
		_delay_ms(150);	
	}
		
	Display_Clear();
	Display_Goto_Position(0, 0);

}


//void port_init() {
    //DDRE = 0xFF;
    //DDRC = (1<<PC0) | (1<<PC1) | (1<<PC2);
    //PORTC = 0;
    //PORTE = 0;
//}
//
//

void setup_dsp_customchars() {
    /*
     * Setup custom character required for gears
     */
    Display_Define_Custom_Character(7,0,0,0,0,0,0x1F,0x1F,0x1F);
    Display_Define_Custom_Character(1,0x1F,0x1F,0,0,0,0,0,0);
    Display_Define_Custom_Character(2,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF,0xEF);
    Display_Define_Custom_Character(3,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E);
    Display_Define_Custom_Character(4,0xFF,0xFF,0xFF,0xFF,0,0,0,0);
    Display_Define_Custom_Character(5,0xFF,0xFF,0xFF,0xFF,0xEF,0xEF,0xEF,0xEF);
    Display_Define_Custom_Character(6,0x08,0x15,0x0A,0x04,0x0A,0x15,0x02,0x00);

}

void setup_dsp_empty(uint8_t screen[4][21]) {
	/*
	 * Fill screen with spaces and properly null terminate each line.
	 */
	for (int i=0; i<4; i++) {
		memset(screen[i], ' ', 20);
		screen[i][20] = '\0';
	}
}

void setup_dsp_debug(uint8_t screen[4][21]) {
    /*
     * Setup the debug display.
     *
     */
	memcpy(screen[0], (uint8_t *)"BPFR=", 5);
	screen[0][7] = 'B';
	
	memcpy(screen[1], (uint8_t *)"BPRE=", 5);
	screen[1][7] = 'B';
	
	memcpy(screen[2], (uint8_t *)"BTL=", 4);
	screen[2][7] = 'C';
	
	memcpy(screen[3], (uint8_t *)"BTR=", 4);
	screen[3][7] = 'C';
	
	// Battery voltage
	memcpy(screen[0]+9, (uint8_t *)"VB=", 3);
	screen[0][14] = '.';
	screen[0][16] = 'V';
	
	memcpy(screen[1]+9, (uint8_t *)"TPS=", 4);
	screen[1][16] = 0x06;
	
	memcpy(screen[2]+9, (uint8_t *)"OiT=", 4);
	screen[2][16] = 'C';
	
	memcpy(screen[3]+9, (uint8_t *)"Sli=", 4);
	screen[1][16] = 0x06;
}


void setup_dsp_production(uint8_t screen[4][21]) {
    /*
     * Setup the production display memory view.
     */

    // Cooling temperature
    memcpy(screen[0], (uint8_t *)"COOL.", 5);  // Not sure if the cast is necessary
    screen[0][14] = 'C';

    // Oil pressure and temperature
    memcpy(screen[1], (uint8_t *)"OEL  .  B", 9);
	/*screen
    screen[1][8] = 'B';*/
    screen[1][14] = 'C';

	memcpy(screen[2],    (uint8_t *)"CLU.", 4);
	memcpy(screen[2]+11, (uint8_t *)".  s", 4);

    memcpy(screen[3]+0,  (uint8_t *)"E ////", 5);
	memcpy(screen[3]+11, (uint8_t *)".  V", 4);
}


int write_dsp(uint8_t screen[4][21]) {
	line++;
	if (line>3) {line=0;}

	Display_Goto_Position(line, 0);

	Display_Write_String(screen[line]);
	
	return 0;  //TODO: Add meaningful return code
}


void refresh_display_with_can_data(int mode, uint8_t screens[][4][21]) {
	/*
	 * Refresh display content with can data.
	 */
	
	uint8_t cmc[8];
	uint8_t ecu0[8];
	uint8_t ecu2[8];
	uint8_t ecu4[8];
	uint8_t swc[8];
	
	uint8_t fuse[8];
	
	can_rx(&can_cmc, cmc);
	can_rx(&can_ecu_0, ecu0);
	can_rx(&can_ecu_2, ecu2);
	can_rx(&can_voltage, ecu4);
	can_rx(&can_swc, swc);
	
	can_rx(&can_fuse, fuse);
	
	if (fuse[7] != 0) {
		status.error = true;
		add_message(&status.error_messages, "! Fuse  ");
	}
	
	refresh_rpm_raw_can(ecu0);
			
	if (mode == DSP_MDE_PROD){

		
		refresh_gear(cmc[0], screens[mode]);
		refresh_oil_temperature(ecu2[3], screens[mode], 1, 10);
		refresh_oil_pressure(ecu2[4], screens[mode], 1, 4);
		
		refresh_cooler_temperature(ecu2+6, screens[mode], 0, 10);
		
		refresh_clutch_speed(swc[1], screens[mode], 2, 10);
		
		refresh_battery(ecu4+2, screens[mode], 3, 9);
		
		if (status.current_message != NULL) {
			memcpy(screens[mode][3]+0, status.current_message, MESSAGE_SIZE);
		} else {
			memcpy(screens[mode][3]+0, "   OK   ", 8);
		}
		
		// Original code uses swc[8] but that must have been wrong...
		// Note: The current SWC doesn't send an error!
		// refresh_error_swc(swc[7], screens[mode], 3, 6);  
		
		// TODO: "K"
	} else {
		refresh_battery(ecu4+2, screens[mode], 0, 12);
		refresh_oil_temperature(ecu2[3], screens[mode], 2, 13);
		refresh_tps(ecu0[2], screens[mode], 1, 13);
	}
}


void refresh_gear(uint8_t gear, uint8_t screen[4][21]) {
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
            screen[1][17] = 0x07; //custom char 1
            screen[1][18] = 0x07; //custom char 1
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
            screen[1][17] = 0x07; //custom char 1
            screen[1][18] = 0x07; //custom char 1
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
            screen[1][18] = 0x07; //custom char 1
            screen[1][19] = 0x07; //custom char 1

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
            screen[1][18] = 0x07; //custom char 1
            screen[1][19] = 0x07; //custom char 1

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


void refresh_rpm_raw_can(uint8_t rpm_raw[2]) {
	/*
     * Set LEDs corresponding to RPM.
	 *
	 */	
	uint16_t rpm = (rpm_raw[1]<<8 | rpm_raw[0]);
	status.rpm = rpm;
	status.car_running = (rpm > MINIMUM_RPM_CAR_ON);
	
	// Linear scale, a logarithmic one would be better
	// The last three LEDs are at ca. 9500, 9100, 9600
	refresh_rpm(rpm);
}

void refresh_rpm(uint16_t rpm) {
	uint8_t led_left = 0;
	uint8_t led_right = 0;
	
	/*if (status.error) {
		return;
	}*/
	
	if (rpm > 0)		     {led_left |= 0b00000001;}
	if (rpm > rpm_ranges[0]) {led_left |= 0b00000010;}
	if (rpm > rpm_ranges[1]) {led_left |= 0b00000100;}
	if (rpm > rpm_ranges[2]) {led_left |= 0b00001000;}
	if (rpm > rpm_ranges[3]) {led_left |= 0b00010000;}
	if (rpm > rpm_ranges[4]) {led_left |= 0b00100000;}
	if (rpm > rpm_ranges[5]) {led_left |= 0b01000000;}
	if (rpm > rpm_ranges[6]) {led_left |= 0b10000000;}
	
	if (rpm > rpm_ranges[7])  {led_right |= 0b00000001;}
	if (rpm > rpm_ranges[8])  {led_right |= 0b00000010;}
	if (rpm > rpm_ranges[9])  {led_right |= 0b00000100;}
	if (rpm > rpm_ranges[10]) {led_right |= 0b00001000;}
	if (rpm > rpm_ranges[11]) {led_right |= 0b00010000;}
	if (rpm >= RPM_MAX)       {led_right |= 0b00100000;}
	
	if ((status.systime & 2048 ) && rpm > rpm_ranges[9]) {
		//led_right &= ~ (0b00111100);
		led_right = 0;
		led_left = 0;
	}
	
	LED_LEFT = led_left;
	LED_RIGHT = (LED_RIGHT & 0b10000000) | led_right;
};


void refresh_clutch_speed(
		uint8_t raw,
		uint8_t screen[4][21],
		int row, int column) {
	/*
	 * Updates the speed of the gearshift.
	 *
	 * Useful on production screen.
	 */
	 
	uint8_t clutch_speed_ds = raw*10/2+5;
	
	screen[row][column] = clutch_speed_ds/10 + 0x30;
	screen[row][column+2] = clutch_speed_ds % 10 + 0x30;
}


void refresh_battery(
        uint8_t battery_raw[2],
        uint8_t screen[4][21], int row, int column) {
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
	
	touch++;

    uint8_t v_bat = ((battery_raw[1] << 8) + battery_raw[0]) * 0.27;
	
	if (v_bat > BATTERY_VOLTAGE_MAX && status.car_running) {
		status.error = 1;
		add_message(&status.error_messages, "! BatV> ");
		
		if (BLINK_SLOW) {
			memcpy(screen[row]+column, "  . ", 4);
			return;
		}
	}
	
    uint8_t v_bat_001 = v_bat % 10;
    uint8_t v_bat_100 = v_bat / 100;
    uint8_t v_bat_010 = (v_bat-v_bat_001-v_bat_100*100) / 10;

    if (v_bat_100 == 0) {
        screen[row][column] = 0x10;
    } else {
        screen[row][column] = 0x30 + v_bat_100;
    }
    screen[row][column+1] = 0x30 + v_bat_010;
    // screen[row][column+2] = '.';
    screen[row][column+3] = 0x30 + v_bat_001;
}
//
//
void refresh_oil_temperature(
        uint8_t oil_temperature,
        uint8_t screen[4][21], int row, int column) {
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
			
	uint8_t output[3];

    if (oil_temperature_100 == 0) {
		output[0] = 0x10;
    } else {
		output[0] = 0x30 + oil_temperature_100;
    }
    if (oil_temperature_100 == 0 && oil_temperature_010 == 0) {
		output[1] = 0x10;
    } else {
       output[1] = 0x30 + oil_temperature_010;
    }
	
    output[2] = 0x30 + oil_temperature_001;
	
	if (oil_temperature >= OIL_TEMPERATURE_MAX) {
		status.error = true;
		
		if (BLINK_SLOW) {
			memcpy(output, "   ", 3);
		}
		
		add_message(&status.error_messages, "! OilT>");
	}
	for (int i=0; i<3; i++) {
		screen[row][column+i] = output[i];
	}
}
//
//
void refresh_oil_pressure(
    uint8_t oil_pressure_raw,
    uint8_t screen[4][21], int row, int column) {
    /*
     * Refresh the oil pressure.
     *
     * oil_pressure_raw = oil_pressure / 6.25
     * Output width 3.
     */
	
	uint8_t output[3];
    uint16_t oil_pressure = oil_pressure_raw * 6.25;
	//oil_pressure = 242
	//Display zeigte 2.5 bar

	output[0] = oil_pressure / 100;  
	output[1] = '.'; 
	output[2] = (oil_pressure - output[0]*100) / 10; 

	output[0] += 0x30; //0x30 means ascii '0'
	output[2] += 0x30; 
		
	/*int i;
	for (i=0; i<3; i++) {
		output[i] += 0x30;
	}*/

    // screen[row][column] = 0x30 + oil_pressure_100;
    // screen[row][column+1] = '.';
    // screen[row][column+2] = 0x30 + oil_pressure_010;
    // screen[row][column+3] = 0x30 + oil_pressure_001;
	
	if (oil_pressure < OIL_PRESSURE_MIN && status.car_running) {
		status.low_oil_pressure = true;
		
		if (status.low_oil_pressure_error) {
			status.error = true;
			add_message(&status.error_messages, "! OilP< ");
			
			if (BLINK_SLOW) {
				memcpy(output, "   ", 3);
			}
		}
	} else {
		status.low_oil_pressure = false;
	}

#ifdef HIGH_OIL_PRESURE_CHECK
	if (oil_pressure > OIL_PRESSURE_MAX) {
		status.high_oil_pressure = true;

		if (status.high_oil_pressure) {
			status.error = true;
			add_message(&status.error_messages, "! OilP> ");
			
			if (BLINK_SLOW) {
				memcpy(output, "   ", 3);
			}
		}
		} else {
		status.high_oil_pressure = false;
	}
#endif
	
	
	screen[row][column] = output[0];
	screen[row][column+2] = output[2];
}
//
//
//
void refresh_cooler_temperature(
        uint8_t cooler_temperature_raw[2],
        uint8_t screen[4][21], int row, int column) {
    /*
     * Refresh the cooler temperature on screen.
     *
     * cooler_temperature is two bytes long.
     *
     * Output width is 3.
     *
     * Currently used on the production screen.
     */

    int16_t cooler_temperature = (cooler_temperature_raw[1] << 8 | cooler_temperature_raw[0]);

    if (cooler_temperature < 0) cooler_temperature = 0;  // it probably cannot freeze

    uint8_t cooler_temperature_100 = cooler_temperature / 100;
    uint8_t cooler_temperature_001 = cooler_temperature % 10;
    uint8_t cooler_temperature_010 = (
            cooler_temperature - cooler_temperature_001 - cooler_temperature_100 * 100) / 10;
			
	if (cooler_temperature >= COOLING_TEMPERATUR_MAX && status.car_running) {
		status.error = true;
		add_message(&status.error_messages, "! CoolT>");
		
		if (BLINK_SLOW) {
			int i;
			for (i=0; i<3; i++) {
				screen[row][column+i] = 0x10;
			}
			return;
		}
	}

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
//
//
//void refresh_rpm(uint8_t screen[4][20], int row, int column) {
    ///*
     //* Refresh the revolutions per minute counter on the screen.
     //*
     //* Currently not used at all. It is not implemented.
     //*/
//}
//
//
//void refresh_break_pressures(uint8_t screen[4][20], int row, int column) {
    ///*
     //* Refresh break pressure on screen.
     //*
     //* Currently only used on the debugging screen.
     //*/
    //// I'm not implementing this now, as I'm not sure how it is "filled in"
//}
//
//
//void refresh_break_temperatures(uint8_t screen[4][20], int row, int column) {
    ///*
     //* Refresh break temperature on screen.
     //*
     //* Currently only used on the debugging screen.
     //*/
    //// I'm not implementing this now, as I'm not sure how it is "filled in"
//}
//
//

void refresh_error_swc(uint8_t error_level, uint8_t screen[4][21], int row, int column) {
    /*
     * Refresh the shown error code sent by SWC.
     *
     * Currently only used on the production screen.
     */
    screen[row][column] = 0x30 + error_level;
}

//
//
void refresh_tps(uint8_t tps_raw, uint8_t screen[4][21], int row, int column) {
    /*
     * Refresh the TPS.
     * TODO: What is that?
     *
     * Currently only used on the debugging screen.
     */
    uint8_t tps = tps_raw / 2;

    uint8_t tps_100 = tps / 100;
    uint8_t tps_001 = tps % 10;
    uint8_t tps_010 = (tps - tps_001 - tps_100 * 100) / 10;

    if (tps_100 == 0) {
        screen[row][column] = 0x10;
    } else {
        screen[row][column] = 0x30 + tps_100;
    }
    if(tps_100 == 0 && tps_010 == 0) {
        screen[row][column+1] = 0x10;
    } else {
        screen[row][column+1] = 0x30 + tps_010;
    }

    screen[row][column+2] = 0x30+tps_001;
}
//
//
//
//void init() {
    ///*
     //* Initializes display, can, ports, ...
     //*/
    //timer_config();
    //sei();  // Enable interrupts on Atmel controllers (e.g. timers)
//
    //can_init();
    //port_init();
//
    //dsp_init();
    //dsp_definechars();
//}
//
//
//int main() {
    //uint8_t * screen[20];
    //uint8_t screen_production [4][20];
    //uint8_t screen_debug[4][20];
//
    //screen = &screen_production;
//
    //init();
//
    //dsp_setup_production(screen_production);
    //dsp_setup_debug(screen_debug);
//
    //CAN_MOB_ID can_data[7] = {
            //{0, CAN_ID_SWC},
            //{1, CAN_ID_CMC},
            //{2, CAN_ID_DATALOGGER_VOLTAGE},
            //{3, CAN_ID_DATALOGGER_BREAKPRESSURE},
            //{4, CAN_ID_ECU0},
            //{5, CAN_ID_ECU2},
            //{6, CAN_ID_ECU4}
    //};
//
    //char row=0;
    //char column=0;
//
    //while (1) {
        //if (counter_update_data >= 44) {
            //counter_update_data = 0;
//
            //can_multi_rx(can_data);
//
            //// change display mode between 'production' and debug
            //dsp_mde = can_data_bytes[1][4] & 0b00001000;
//
            //if (dsp_mde>1) dsp_mde=1;
//
            //if (dsp_mde == 0) {  // production
                //screen = &screen_production;
                //// First byte of SWC data is the gear
                //refresh_gear(CAN_DATA_BYTES[1][0], screen_production);
//
                //// Refresh other status information.
                //refresh_oil_pressure(CAN_DATA_BYTES[4][4], screen_production, 1, 4);
                //refresh_oil_temperature(CAN_DATA_BYTES[4][3], screen_production, 1, 10);
                //refresh_cooler_temperature(CAN_DATA_BYTES[4]+6, screen_production, 0, 10);
                //refresh_error_swc(CAN_DATA_BYTES[0][8], screen_production, 3, 7);
//
            //} else {  // debug
                //screen = &screen_debug;
                //// Battery bytes are at
                //// [6][0] and [6][1]
                //refresh_battery(CAN_DATA_BYTES[6]+1, screen_debug, 0, 12);
                //refresh_oil_temperature(CAN_DATA_BYTES[4][3], screen_debug, 2, 13);
                ////refresh_break_pressures()
                //refresh_tps(CAN_DATA_BYTES[4][2], screen_debug, 1, 13)
            //}
//
        //}
//
        //dsp_write(*screen);
    //}
//
//}