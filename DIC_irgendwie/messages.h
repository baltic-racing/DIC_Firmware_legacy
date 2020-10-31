/*
 * messages.h
 *
 * Created: 06.07.2019 15:30:45
 *  Author: Simon
 */ 


#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <stdint.h>
#include <stdbool.h>

#define MESSAGE_SIZE (8)

struct Message {
	uint8_t message[MESSAGE_SIZE+1];
	struct Message *next;
};

struct MessageList {
	struct Message *start; 	
};

void add_message(struct MessageList* list, uint8_t message[MESSAGE_SIZE]);
int remove_message(struct MessageList* list, uint8_t message[MESSAGE_SIZE]);
struct Message* find_message(struct MessageList* list, uint8_t message[MESSAGE_SIZE]);

uint8_t* get_next_message(struct MessageList* list);


#endif /* ERROR_QUEUE_H_ */