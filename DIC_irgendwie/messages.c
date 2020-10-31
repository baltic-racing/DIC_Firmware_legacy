/*
 * messages.c
 *
 * Created: 06.07.2019 15:30:25
 *  Author: Simon
 */ 

#include "messages.h"
#include <stdlib.h>
#include <string.h>

void add_message(struct MessageList* list, uint8_t message[MESSAGE_SIZE]) {
	struct Message* new_message = (struct Message*) malloc(sizeof(struct Message));
	// memcpy(new_message->message, message, MESSAGE_SIZE);
	
	if (find_message(list, message) != NULL) {
		return;
	}
	
	memcpy(new_message->message, message, MESSAGE_SIZE);
	new_message->message[MESSAGE_SIZE] = '\0';
	
	if (list->start == NULL) {
		new_message->next = new_message;
		list->start = new_message;
	} else {
		new_message->next = list->start->next;  // old message after start
		list->start->next = new_message;        // add new message after start
	}
}

/*
int remove_message(struct MessageList* list, uint8_t message[MESSAGE_SIZE]) {
	struct Message* position = find_message(list, message);
	
	if (position == NULL) {
		return 0;
	}
	
	// One element list
	if (position->next == position) {
		list->start = NULL;
	}
	
	
	free(position->next);
	return 1;
};
*/

// Returns the message pointing to the searched message!
struct Message* find_message(struct MessageList* list, uint8_t message[MESSAGE_SIZE]) {
	if (list->start == NULL) {
		return NULL;
	}
	
	struct Message* position = list->start;
	while (position->next != list->start) {
		if (memcmp(position->next->message, message, MESSAGE_SIZE) == 0) {
			return position;
		}
		
		position = position->next;
	}
	
	return NULL;
};

uint8_t* get_next_message(struct MessageList* list) {
	if (list->start == NULL) {
		return NULL;
	}
	
	list->start = list->start->next;
	return list->start->message;
}



