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
	// check if the pointer exists
	channel_t *pointer = channels->head; 
	while(pointer != NULL){
		char *pointer_name = (char *)pointer->name;
		if(strcmp(pointer_name, name) == 0){
			return pointer; 
		}
		pointer = pointer->next;
	}

	channel_t *c = (channel_t *) malloc(sizeof(channel_t)); 
	c->head = NULL; 
	c->tail = NULL; 
	c->last_msg = -1; 
	c->next = NULL; 
	c->name = (channel_id_t) name; 

	if(channels->head == NULL){
		channels->head = c; 
		channels->tail = c; 
	}else{
		channels->tail->next = c; 
		channels->tail = c; 
	}


    return NULL;
}

channel_t *get_channel(channel_list_t *channels, const char *name) {
	channel_t *pointer = channels->head; 
	while(pointer != NULL){
		char *pointer_name = pointer->name;
		if(strcmp(pointer_name, name) == 0){
			return pointer; 
		}
		pointer = pointer->next;
	}
	return NULL;
}


void free_channels(channel_list_t *channels) {

}

void add_message(channel_t *channel, const char *text) {
	
	message_t *m = (message_t *) malloc(sizeof(message_t));
	m->text = text;
	if(channel->head == NULL){ 
		m->id = (message_id_t) 0;
		channel->head = m;
		channel->tail = m;
		channel->last_msg = m->id;
	}else{
		m->id = channel->tail->id + 1;
		channel->tail->next = m; 
		channel->tail = m;
		channel->last_msg = m->id;
	}

}

message_t *get_message(channel_t *channel, message_id_t id) {
	message_t *pointer = channel->head; 
	while(pointer != NULL){
		if(pointer->id == id){
			return pointer;
		}
		pointer = pointer->next; 
	}
	return NULL; 
}

void dump(channel_list_t *channels) {
	for (channel_t *c = channels->head; c != NULL; c = c->next) {
		printf("Channel %s\n", c->name);
		for (message_t *m = c->head; m != NULL; m = m->next) {
			printf("\t[%llu]: %s\n", m->id, m->text);
		}
		printf("\n\n");
	}
}

