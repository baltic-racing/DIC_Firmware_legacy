
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

uint8_t Rotary_Encoder_Right = 0;
uint16_t rpm = 0;

can_init_messages(){
struct CAN_MOB can_SWC_mob;
can_SWC_mob.mob_id = 0x100;
can_SWC_mob.mob_idmask = 0xffff;
can_SWC_mob.mob_number = 0;
uint8_t SWC_databytes[8];

struct CAN_MOB can_CMC_mob;
can_CMC_mob.mob_id = 0x200;
can_CMC_mob.mob_idmask = 0xffff;
can_CMC_mob.mob_number = 1;
uint8_t CMC_databytes[8];

struct CAN_MOB can_ETC_mob;
can_ETC_mob.mob_id = 0x201;
can_ETC_mob.mob_idmask = 0xffff;
can_ETC_mob.mob_number = 2;
uint8_t ETC_databytes[8];

struct CAN_MOB can_ecu0_mob;
can_ecu0_mob.mob_id = 0x600;
can_ecu0_mob.mob_idmask = 0xffff;
can_ecu0_mob.mob_number = 3;
uint8_t ecu0_databytes[8];
struct CAN_MOB can_ecu2_mob;
can_ecu2_mob.mob_id = 0x602;
can_ecu2_mob.mob_idmask = 0xffff;
can_ecu2_mob.mob_number = 4;
uint8_t ecu2_databytes[8];

struct CAN_MOB can_ecu4_mob;
can_ecu4_mob.mob_id = 0x604;
can_ecu4_mob.mob_idmask = 0xffff;
can_ecu4_mob.mob_number = 5;
uint8_t ecu4_databytes[8];

struct CAN_MOB can_SHR0_mob;
can_SHR0_mob.mob_id = 0x500;
can_SHR0_mob.mob_idmask = 0xffff;
can_SHR0_mob.mob_number = 6;
uint8_t SHR0_databytes[8];

struct CAN_MOB can_SHL0_mob;
can_SHL0_mob.mob_id = 0x503;
can_SHL0_mob.mob_idmask = 0xffff;
can_SHL0_mob.mob_number = 7;
uint8_t SHL0_databytes[8];

struct CAN_MOB can_Logger0_mob;
can_Logger0_mob.mob_id = 0x799;
can_Logger0_mob.mob_idmask = 0xffff;
can_Logger0_mob.mob_number = 8;
uint8_t Logger0_databytes[8];

struct CAN_MOB can_Logger1_mob;
can_Logger1_mob.mob_id = 0x798;
can_Logger1_mob.mob_idmask = 0xffff;
can_Logger1_mob.mob_number = 9;
uint8_t Logger1_databytes[8];

struct CAN_MOB can_Logger2_mob;
can_Logger2_mob.mob_id = 0x797;
can_Logger2_mob.mob_idmask = 0xffff;
can_Logger2_mob.mob_number = 10;
uint8_t Logger2_databytes[8];
}

CAN_recieve(){
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
	
	Rotary_Encoder_Right = SWC_databytes[0];
	rpm = ecu0_databytes[1] << 8 | ecu0_databytes[0];
}