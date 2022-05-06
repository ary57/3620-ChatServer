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
    return NULL;
}

channel_t *get_channel(channel_list_t *channels, const char *name) {
	return NULL;
}


void free_channels(channel_list_t *channels) {}

void add_message(channel_t *channel, const char *text) {
}

message_t *get_message(channel_t *channel, message_id_t id) {
}

void dump(channel_list_t *channels) {
	for (channel_t *c = channels->head; c != NULL; c = c->next) {
		printf("Channel %s\n", c->name);
		for (message_t *m = c->head; m != NULL; m = m->next) {
			printf("\t[%lu]: %s\n", m->id, m->text);
		}
		printf("\n\n");
	}
}

