#include "storage.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

channel_list_t *channel_list = NULL;

channel_list_t *get_channels() {
	if (channel_list == NULL) {
		channel_list = (channel_list_t *) malloc(sizeof(channel_list_t));
		channel_list->head = NULL;
		channel_list->tail = NULL;
	}
	
    return channel_list;
}

channel_t *create_channel(channel_list_t *channels, const char *name) {
	pthread_mutex_lock(&channels->lock);
	channel_t *pointer = channels->head;

	while(pointer != NULL){
		char *ptrName = (char *) pointer->name; 
		if(strcmp(ptrName, name) == 0){
			// printf("%s: %s\n", "Duplicate Found", name);
			pthread_mutex_unlock(&channels->lock);
			return pointer; 
		}
		pointer = pointer->next;
	}

	channel_t *c = (channel_t *) malloc(sizeof(channel_t));
	c->name = (channel_id_t) name; 
	c->next = NULL; // should this be null? once this is added to the end of the list, it's next should be null.
	c->head = NULL; // Head of the messages linked list. Should be null initially?
	c->tail = NULL; // End of messages linked list. Should be null initially?
	c->last_msg = 0; // is this just the message id of the message before it? if it is, what should be the initial value for the first message in the messages? -- resolved; is the 'length' of the linked list

	if(channels->head == NULL){
		channels->head = c; 
		channels->tail = c; 
		c->last_msg = 0;
	}else{
		c->last_msg = channels->tail->last_msg + 1;
		channels->tail->next = c; 
		channels->tail = c;
	}
	pthread_mutex_unlock(&channels->lock);

    return c;
}

channel_t *get_channel(channel_list_t *channels, const char *name) {
	pthread_mutex_lock(&channels->lock);

	channel_t *pointer = channels->head;
	while(pointer != NULL){
		if( strcmp(pointer->name, name) == 0) {
			pthread_mutex_unlock(&channels->lock);

			return pointer; 
		}
		pointer = pointer->next; 
	}

	pthread_mutex_unlock(&channels->lock);
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
	channel->head = NULL; 
	channel->tail = NULL; 
}


void free_channels(channel_list_t *channels) {
	channel_t *toFree = channels->head;
	while(toFree != NULL){
		channel_t *toFreeNext = toFree->next; 
		free_messages(toFree);
		free(toFree); 
		toFree = toFreeNext; 
	}
	channels->head = NULL; 
	channels->tail = NULL; 
}

// void free_channels(channel_list_t *channels){
// 	channel_t *toFree = channels->head; 
// 	while(toFree != NULL){
// 		channel_t *toFreeNext = toFree->next;
		
// 		message_t *messageToFree = toFree->head;
// 		while(messageToFree != NULL){
// 			message_t *messageToFreeNext = messageToFree->next; 
// 			free(messageToFree);
// 			messageToFree = messageToFreeNext;
// 		}
// 		free(toFree);
// 		toFree = toFreeNext;
// 	}
// }

void add_message(channel_t *channel, const char *text) {
	pthread_mutex_lock(&channel->lock);

	message_t *m = (message_t *) malloc(sizeof(message_t));
	m->text = text;
	m->next = NULL; 
	if(channel->head == NULL){ 
		m->id = (message_id_t) 0;
		channel->head = m;
		channel->tail = m; 
	}else{
		m->id = channel->tail->id + 1;
		channel->tail->next = m; 
		channel->tail = m;
	}

	pthread_mutex_unlock(&channel->lock);
}

message_t *get_message(channel_t *channel, message_id_t id) {
	pthread_mutex_lock(&channel->lock);

	message_t *pointer = channel->head; 
	while(pointer != NULL){
		if(pointer->id == id){
			
			pthread_mutex_unlock(&channel->lock);

			return pointer;
		}
		pointer = pointer->next; 
	}
	pthread_mutex_unlock(&channel->lock);
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
 * Note to self: the segmentation fault in test_step_1.c makes sense. It dumps first, then frees it then prints it by accessing the freed stuff again. 
 * LINE 134, should be 100 NOT 10. 
*/

/*
100 -86 A
86-72   B
72-44   C
44-0    D
*/

/*
 A: 86+ 
 B: 86-72
 C: 72-44
 D 44-0
*/

/*
	pthread detach -> use this apparaently
	basically don't use join, use detach instead. 
*/