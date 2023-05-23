
/*
 * CAN_Data.c
 *
 * Created: 09.05.2021 17:36:25
 *  Author: Lukas
 */ 

//This file is designed to give you updated CAN data, once called all the Values taken from the CAN-Bus are updated
//If you need additional data from the CAN please implement it in here so it gets automatically updated when the CAN_REcieve functions is called every 10ms in the main.c
#include <avr/io.h>

#include "CAN_Data.h"
#include "canlib.h"

extern uint8_t dsp_mde;

uint8_t Rotary_Encoder_Right = 0;
uint8_t Rotary_Encoder_Left = 0;
uint16_t TSVoltage = 0;
uint16_t AccumulatorTemperature = 0;
uint16_t SOC = 0;
uint16_t LVSVoltage = 0;
uint16_t CLT = 0;
uint8_t BrakeBias = 0;
uint16_t INV1Voltage = 0; //noch nicht implementiert
uint16_t INV2Voltage = 0; //noch nicht implementiert
uint8_t APPS1 = 0;
uint8_t APPS2 = 0;
uint8_t BPF = 0;
uint8_t BPR = 0;
uint8_t SDCIFB = 0;
uint16_t FRO = 0;
uint8_t AMS_ERR = 0;
uint16_t ERR_info = 0;
uint8_t IMD_ERR = 0;
uint8_t TS_RDY = 0;

uint8_t LapNumber = 0;
uint32_t Laptime = 0;
uint8_t Laptime_1 = 0;
uint16_t Laptime_2 = 0;
uint32_t Laptime_3 = 0;
uint32_t Laptime_4 = 0;
uint32_t Besttime = 0;
uint8_t Besttime_1 = 0;
uint16_t Besttime_2 = 0;
uint32_t Besttime_3 = 0;
uint32_t Besttime_4 = 0;
uint32_t Pred_time = 0;
uint8_t Pred_time_1 = 0;
uint16_t Pred_time_2 = 0;
uint32_t Pred_time_3 = 0;
uint32_t Pred_time_4 = 0;
uint16_t ODO = 0;
uint16_t GPS_Speed = 0;

extern LC_Active;

struct CAN_MOB can_SWC_mob;
	uint8_t SWC_databytes[8];	
	
struct CAN_MOB can_Fusebox0_mob;
	uint8_t Fusebox0_databytes[8];
struct CAN_MOB can_Fusebox1_mob;
	uint8_t Fusebox1_databytes[8];
	
struct CAN_MOB can_SHR0_mob;
	uint8_t SHR0_databytes[8];	
	
struct CAN_MOB can_SHL0_mob;
	uint8_t SHL0_databytes[8];
	
struct CAN_MOB can_SHB0_mob;
	uint8_t SHB0_databytes[8];
	
struct CAN_MOB can_Logger0_mob;
	uint8_t Logger0_databytes[8];	
struct CAN_MOB can_Logger1_mob;
	uint8_t Logger1_databytes[8];
struct CAN_MOB can_Logger2_mob;
	uint8_t Logger2_databytes[8];
	
struct CAN_MOB can_DIC0_mob;
	uint8_t DIC0_databytes[8];
	
struct CAN_MOB can_AMS0_mob;
	uint8_t AMS0_databytes[8];
struct CAN_MOB can_AMS1_mob;
	uint8_t AMS1_databytes[8];
struct CAN_MOB can_AMS2_mob;
	uint8_t AMS2_databytes[8];
struct CAN_MOB can_AMS3_mob;
	uint8_t AMS3_databytes[8];

void can_init_messages(){
	
	can_SWC_mob.mob_id = 0x800;
	can_SWC_mob.mob_idmask = 0xffff;
	can_SWC_mob.mob_number = 0;
	
	can_Fusebox0_mob.mob_id = 0x600;
	can_Fusebox0_mob.mob_idmask = 0xffff;
	can_Fusebox0_mob.mob_number = 1;
	
	can_Fusebox0_mob.mob_id = 0x601;
	can_Fusebox0_mob.mob_idmask = 0xffff;
	can_Fusebox0_mob.mob_number = 2;
	
	can_SHR0_mob.mob_id = 0x400;
	can_SHR0_mob.mob_idmask = 0xffff;
	can_SHR0_mob.mob_number = 3;
	
	can_SHL0_mob.mob_id = 0x410;
	can_SHL0_mob.mob_idmask = 0xffff;
	can_SHL0_mob.mob_number = 4;
	
	can_SHB0_mob.mob_id = 0x420;
	can_SHB0_mob.mob_idmask = 0xffff;
	can_SHB0_mob.mob_number = 5;
	
	can_Logger0_mob.mob_id = 0x799;
	can_Logger0_mob.mob_idmask = 0xffff;
	can_Logger0_mob.mob_number = 6;
	
	can_Logger1_mob.mob_id = 0x798;
	can_Logger1_mob.mob_idmask = 0xffff;
	can_Logger1_mob.mob_number = 7;
		
	can_Logger2_mob.mob_id = 0x797;
	can_Logger2_mob.mob_idmask = 0xffff;
	can_Logger2_mob.mob_number = 10;
	
	can_DIC0_mob.mob_id = 0x500;
	can_DIC0_mob.mob_idmask = 0xffff;
	can_DIC0_mob.mob_number = 11;
	
	can_AMS0_mob.mob_id = 0x200;
	can_AMS0_mob.mob_idmask = 0xffff;
	can_AMS0_mob.mob_number = 12;
	
	can_AMS1_mob.mob_id = 0x201;
	can_AMS1_mob.mob_idmask = 0xffff;
	can_AMS1_mob.mob_number = 13;
	
	can_AMS2_mob.mob_id = 0x202;
	can_AMS2_mob.mob_idmask = 0xffff;
	can_AMS2_mob.mob_number = 14;
	
	can_AMS3_mob.mob_id = 0x203;
	can_AMS3_mob.mob_idmask = 0xffff;
	can_AMS3_mob.mob_number = 15;	
	
}	

void CAN_recieve(){
	
	can_rx(&can_SWC_mob, SWC_databytes);
	can_rx(&can_Fusebox0_mob, Fusebox0_databytes);
	can_rx(&can_Fusebox1_mob, Fusebox1_databytes);
	can_rx(&can_SHR0_mob, SHR0_databytes);
	can_rx(&can_SHL0_mob, SHL0_databytes);
	can_rx(&can_SHB0_mob, SHB0_databytes);
	can_rx(&can_Logger0_mob, Logger0_databytes);
	can_rx(&can_Logger1_mob, Logger1_databytes);
	can_rx(&can_Logger2_mob, Logger2_databytes);
	can_rx(&can_AMS0_mob, AMS0_databytes);
	can_rx(&can_AMS1_mob, AMS1_databytes);
	can_rx(&can_AMS2_mob, AMS2_databytes);
	can_rx(&can_AMS3_mob, AMS3_databytes);
	
	can_tx(&can_DIC0_mob, DIC0_databytes);
	
}

void CAN_put_data(){
		// Testdata comment when display is in car
		/*
		SWC_databytes[0] = 0;
		SWC_databytes[1] = 1;

		//APPS Values in 0,1%
		SHR0_databytes[0] = 400;
		SHR0_databytes[1] = 400 >> 8;
		SHR0_databytes[2] = 450;
		SHR0_databytes[3] = 450 >> 8;
		
		// Brake Pressures in 0,1Bar
		SHL0_databytes[0] = 350;
		SHL0_databytes[1] = 350 >> 8;
		SHL0_databytes[2] = 380;
		SHL0_databytes[3] = 380 >> 8;
		
		// CLT in 0,1°C
		SHB0_databytes[0] = 540;
		SHB0_databytes[1] = 540 >> 8;	
		
		//Board Voltage Fusebox in 0,025V
		Fusebox0_databytes[0] = 920;
		Fusebox0_databytes[1] = 920 >> 8;
		
		//Tractive System Voltage in 0,1667V
		AMS1_databytes[0] = 3065;
		AMS1_databytes[1] = 3065 >> 8;
		
		//SOC in 1%
		AMS1_databytes[4] = 96;
		AMS1_databytes[5] = 96 >> 8;
		
		//Cell temperature in 0,1°C
		AMS2_databytes[7] = 560;
		AMS2_databytes[8] = 560 >> 8;	
		
		//Cell Voltage min in 0,005V
		AMS2_databytes[0] = 820;
		AMS2_databytes[1] = 820 >> 8;
		
		//Laptime
		Logger0_databytes[1] = 14985;
		Logger0_databytes[2] = 14985>>8;
		Logger0_databytes[3] = 14985>>16;
		Logger0_databytes[4] = 14985>>24;
		//Besttime
		Logger1_databytes[0] = 16554;
		Logger1_databytes[1] = 16554>>8;
		Logger1_databytes[2] = 16554>>16;
		Logger1_databytes[3] = 16554>>24;
		//predtime
		Logger1_databytes[4] = 96200;
		Logger1_databytes[5] = 96200>>8;
		Logger1_databytes[6] = 96200>>16;
		Logger1_databytes[7] = 96200>>24;
		
		Logger0_databytes[5] = 223;
		Logger0_databytes[6] = 223 << 8;
		
		Logger2_databytes[6] = 96;
		Logger2_databytes[7] = 96<<8;
		
		Logger0_databytes[0] = 42;
		*/
		
		
		
	Rotary_Encoder_Right = SWC_databytes[0];
	Rotary_Encoder_Left = SWC_databytes[1];
	dsp_mde = Rotary_Encoder_Right%4;
	BrakeBias = 0;

	APPS1 = (SHR0_databytes[1] << 8 | SHR0_databytes[0])/10;
	APPS2 = (SHR0_databytes[3] << 8 | SHR0_databytes[2])/10;
	
	BPF = (SHL0_databytes[1] << 8 | SHL0_databytes[0])/10;
	BPR = (SHL0_databytes[3] << 8 | SHL0_databytes[2])/10;
	CLT = (SHB0_databytes[1] << 8 | SHB0_databytes[0])/10;

	LVSVoltage = (Fusebox0_databytes[1] << 8 | Fusebox0_databytes[0])/40;
	SDCIFB = (Fusebox1_databytes[1] << 8 | Fusebox1_databytes[0]);
	FRO = (Fusebox1_databytes[3] << 8 | Fusebox1_databytes[2]);
	
	AMS_ERR = AMS0_databytes[0];
	ERR_info = (AMS0_databytes[2] << 8 | AMS0_databytes[1]);
	IMD_ERR = AMS0_databytes[4];
	TSVoltage = (AMS1_databytes[1] << 8 | AMS1_databytes[0])/6;
	SOC = (AMS1_databytes[5] << 8 | AMS1_databytes[4]);
	AccumulatorTemperature = (AMS2_databytes[7] << 8 | AMS2_databytes[6]);
	TS_RDY = AMS3_databytes[0];

	//For some weird reason the length of the variable that gets shifted seems to matter, atleast in this usecase, with for e.g BPF etc it works fine which may be becouse we are actually only transmitting an 8 bit value
	//what happens is we shift an 8bit uint by 8 to the left and want to save it in an 32bit uint, we put for e.g 135 in and should get 34560. instead we are getting 4294965720 which is definitly also not an 8bit value
	//i do not know what is happening here but shifting a variable beyond its own limit seems to do weird stuff. therefore this stupid hacky fix with putting the 8bit integers into 32bit integer and than whacking it together
	
	Laptime_1 = Logger0_databytes[1];
	Laptime_2 = Logger0_databytes[2];
	Laptime_3 = Logger0_databytes[3];
	Laptime_4 = Logger0_databytes[4];
	
	Laptime = Laptime_1 | Laptime_2 << 8 | Laptime_3 << 16 | Laptime_4 << 24;
	
	Besttime_1 = Logger1_databytes[0];
	Besttime_2 = Logger1_databytes[1];
	Besttime_3 = Logger1_databytes[2];
	Besttime_4 = Logger1_databytes[3];
	
	Besttime = Besttime_1 | Besttime_2 << 8 | Besttime_3 << 16 | Besttime_4 << 24;
	
	Pred_time_1 = Logger1_databytes[4];
	Pred_time_2 = Logger1_databytes[5];
	Pred_time_3 = Logger1_databytes[6];
	Pred_time_4 = Logger1_databytes[7];
	
	Pred_time = Pred_time_1 | Pred_time_2 << 8 | Pred_time_3 << 16 | Pred_time_4 << 24;
	
	ODO = Logger0_databytes[6] << 8 | Logger0_databytes[5];
	GPS_Speed = Logger2_databytes[7] << 8 | Logger2_databytes[6];
	LapNumber = Logger0_databytes[0];	
	
	DIC0_databytes[0] = LC_Active;
	
}