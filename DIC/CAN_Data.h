
/*
 * CAN_Data.h
 *
 * Created: 09.05.2021 17:36:36
 *  Author: Lukas
 */ 

//This file is designed to give you updated CAN data, once called all the Values taken from the CAN-Bus are updated
//If you need additional data from the CAN please implement it in here so it gets automatically updated when the CAN_Recieve functions is called every 10ms in the main.c
//Important! keep  in mind that the AT90CAN can by default only handle 16 CAN Messages, either try to stay within this limit, or create a buffer where you put the CAN data in and the reconfigure
//this will require modification of the CANlib so handle with care

#include <avr/io.h>


void CAN_recieve();
void can_init_messages();