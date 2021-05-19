
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
	// Testdata comment when display is in car
	ecu4_databytes[2] = 128;
	ecu4_databytes[3] = 128 >> 8;
	CMC_databytes[0] = 1;
	ecu2_databytes[6] = 90;
	ecu2_databytes[7] = 90 >> 8;
	ecu2_databytes[4] = 23;
	ecu2_databytes[3] = 110;	
	SWC_databytes[0] = 1;
	SWC_databytes[1] = 0;
	
	
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
}