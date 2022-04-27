#include "storage.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

channel_list_t *channel_list = NULL;

channel_list_t *get_channels() {
	if (channel_list == NULL) {
		channel_list = (channel_list_t *) malloc(sizeof(channel_list_t));
		channel_list->head = NULL;
		channel_list->head = NULL;
	}
	
    return channel_list;
}

channel_t *create_channel(channel_list_t *channels, const char *name) {
	channel_t *pointer = channels->head;

	// if(pointer == NULL) printf("%s\n", "NAY");

	while(pointer != NULL){
		char *ptrName = (char *) pointer->name; 
		if(strcmp(ptrName, name) == 0){
			printf("%s: %s\n", "Duplicate Found", name);
			return pointer; 
		}
		pointer = pointer->next;
	}

	channel_t *c = (channel_t *) malloc(sizeof(channel_t));
	c->name = (channel_id_t) name; 
	c->next = NULL; // should this be null? once this is added to the end of the list, it's next should be null.
	c->head = NULL; // Head of the messages linked list. Should be null initially?
	c->tail = NULL; // End of messages linked list. Should be null initially?
	c->last_msg = -1; 

	if(channels->head == NULL){
		channels->head = c; 
		channels->tail = c; 
	}else{
		channels->tail->next = c; 
		channels->tail = c; 
	}

    return c;
}

channel_t *get_channel(channel_list_t *channels, const char *name) {
	channel_t *pointer = channels->head;
	while(pointer != NULL){
		if( strcmp(pointer->name, name) == 0) return pointer; 
		pointer = pointer->next; 
	}
	return NULL;
}

// helper function: free messages. Frees all the messages from a channel
void free_messages(channel_t *channel){
	message_t *toFree = channel->head;
	while(toFree != NULL){
		message_t *toFreeNext = toFree->next; 
		free(toFree); 
		toFree = toFreeNext; 
	}
}


void free_channels(channel_list_t *channels) {
	// channel_t *toFree = channels->head;
	// while(toFree != NULL){
	// 	channel_t *toFreeNext = toFree->next; 
	// 	free_messages(toFree);
	// 	free(toFree); 
	// 	toFree = toFreeNext; 
	// }
}

void add_message(channel_t *channel, const char *text) {
	message_t *m = (message_t *) malloc(sizeof(message_t));
	m->text = text; 
	if(channel->head == NULL){ 
		m->id = (message_id_t) 0; 
		channel->head = m;
		channel->tail = m; 
	}else{
		m->id = channel->tail->id + 1;
		channel->tail->next = m; 
		channel->tail = m;
	}
}

message_t *get_message(channel_t *channel, message_id_t id) {
	message_t *pointer = channel->head; 
	while(pointer != NULL){
		if(pointer->id == id) return pointer;  // is there a strcmp version of this or is == ok to compare?
		pointer = pointer->next; 
	}
	return NULL; 
}

void dump(channel_list_t *channels) {
	for (channel_t *c = channels->head; c != NULL; c = c->next) {
		printf("Channel %s\n", c->name);
		for (message_t *m = c->head; m != NULL; m = m->next) {
			// original: printf("\t[%lu]: %s\n", m->id, m->text);
			printf("\t[%llu]: %s\n", m->id, m->text); 
		}
		printf("\n\n");
	}
}

/*
 * Note to self: the segmentation fault in test_step_1.c makes sense. It dumps first, then frees it then prints it again. 
*/