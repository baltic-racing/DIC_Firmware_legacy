
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
uint16_t rpm = 0;
uint8_t gear = 0;
uint8_t CLT = 0;
uint8_t OILP = 0;
uint8_t OILT = 0;
uint8_t BrakeBias = 0;
uint8_t Clutchtime = 0;
uint8_t ECUVoltage = 0;
uint8_t TPS1 = 0;
uint8_t TPS2 = 0;
uint8_t TPSE = 0;
uint8_t APPS1 = 0;
uint8_t APPS2 = 0;
uint8_t BPF = 0;
uint8_t BPR = 0;
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

struct CAN_MOB can_SWC_mob;
	uint8_t SWC_databytes[8];	
struct CAN_MOB can_CMC_mob;
	uint8_t CMC_databytes[8];	
struct CAN_MOB can_ETC_mob;
	uint8_t ETC_databytes[8];	
struct CAN_MOB can_ecu0_mob;
	uint8_t ecu0_databytes[8];	
struct CAN_MOB can_ecu2_mob;
	uint8_t ecu2_databytes[8];	
struct CAN_MOB can_ecu4_mob;
	uint8_t ecu4_databytes[8];	
struct CAN_MOB can_SHR0_mob;
	uint8_t SHR0_databytes[8];	
struct CAN_MOB can_SHL0_mob;
	uint8_t SHL0_databytes[8];	
struct CAN_MOB can_Logger0_mob;
	uint8_t Logger0_databytes[8];	
struct CAN_MOB can_Logger1_mob;
	uint8_t Logger1_databytes[8];
struct CAN_MOB can_Logger2_mob;
	uint8_t Logger2_databytes[8];

void can_init_messages(){
	
	can_SWC_mob.mob_id = 0x100;
	can_SWC_mob.mob_idmask = 0xffff;
	can_SWC_mob.mob_number = 0;
	
	can_CMC_mob.mob_id = 0x200;
	can_CMC_mob.mob_idmask = 0xffff;
	can_CMC_mob.mob_number = 1;
	
	can_ETC_mob.mob_id = 0x201;
	can_ETC_mob.mob_idmask = 0xffff;
	can_ETC_mob.mob_number = 2;
	
	can_ecu0_mob.mob_id = 0x600;
	can_ecu0_mob.mob_idmask = 0xffff;
	can_ecu0_mob.mob_number = 3;
	
	can_ecu2_mob.mob_id = 0x602;
	can_ecu2_mob.mob_idmask = 0xffff;
	can_ecu2_mob.mob_number = 4;
	
	can_ecu4_mob.mob_id = 0x604;
	can_ecu4_mob.mob_idmask = 0xffff;
	can_ecu4_mob.mob_number = 5;
	
	can_SHR0_mob.mob_id = 0x500;
	can_SHR0_mob.mob_idmask = 0xffff;
	can_SHR0_mob.mob_number = 6;
	
	can_SHL0_mob.mob_id = 0x503;
	can_SHL0_mob.mob_idmask = 0xffff;
	can_SHL0_mob.mob_number = 7;
	
	can_Logger0_mob.mob_id = 0x799;
	can_Logger0_mob.mob_idmask = 0xffff;
	can_Logger0_mob.mob_number = 8;
	
	can_Logger1_mob.mob_id = 0x798;
	can_Logger1_mob.mob_idmask = 0xffff;
	can_Logger1_mob.mob_number = 9;
		
	can_Logger2_mob.mob_id = 0x797;
	can_Logger2_mob.mob_idmask = 0xffff;
	can_Logger2_mob.mob_number = 10;
}	

void CAN_recieve(){
	can_rx(&can_SWC_mob, SWC_databytes);
	can_rx(&can_CMC_mob, CMC_databytes);
	can_rx(&can_ETC_mob, ETC_databytes);
	can_rx(&can_ecu0_mob, ecu0_databytes);
	can_rx(&can_ecu2_mob, ecu2_databytes);
	can_rx(&can_ecu4_mob, ecu4_databytes);
	can_rx(&can_SHR0_mob, SHR0_databytes);
	can_rx(&can_SHL0_mob, SHL0_databytes);
	can_rx(&can_Logger0_mob, Logger0_databytes);
	can_rx(&can_Logger1_mob, Logger1_databytes);
	can_rx(&can_Logger2_mob, Logger2_databytes);
}

void CAN_put_data(){
		// Testdata comment when display is in car
		ecu4_databytes[2] = 128;
		ecu4_databytes[3] = 128 >> 8;
		CMC_databytes[0] = 1;
		ecu2_databytes[6] = 115;
		ecu2_databytes[7] = 115 >> 8;
		ecu2_databytes[4] = 6;
		ecu2_databytes[3] = 110;
		SWC_databytes[0] = 0;
		SWC_databytes[1] = 0;
		ecu0_databytes[0] = 3500;
		ecu0_databytes[1] = 3500 >> 8;
		ETC_databytes[1] = 60;
		ETC_databytes[2] = 75;
		ETC_databytes[3] = 100;
		ETC_databytes[4] = 105;
		ecu0_databytes[2] = 9;
		SHR0_databytes[0] = 40;
		SHR0_databytes[1] = 40 >> 8;
		SHL0_databytes[2] = 35;
		SHL0_databytes[3] = 35 >> 8;
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
		
	Rotary_Encoder_Right = SWC_databytes[0];
	Rotary_Encoder_Left = SWC_databytes[1];
	dsp_mde = Rotary_Encoder_Right;
	Clutchtime = Rotary_Encoder_Left*5;//0,5s steps
	rpm = ecu0_databytes[1] << 8 | ecu0_databytes[0];
	gear = CMC_databytes[0];
	CLT = ecu2_databytes[7] << 8 | ecu2_databytes[6];
	OILP = ecu2_databytes[4];
	OILT = ecu2_databytes[3];
	BrakeBias = 0;
	ECUVoltage = ecu4_databytes[3] << 8 | ecu4_databytes[2];
	
	TPS1 = ETC_databytes[2];
	TPS2 = ETC_databytes[3];
	TPSE = ecu0_databytes[2];
	APPS1 =ETC_databytes[1];
	APPS2 = ETC_databytes[2];
	BPF = SHR0_databytes[1] << 8 | SHR0_databytes[0];
	BPR = SHL0_databytes[3] << 8 | SHL0_databytes[2];
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
	
}