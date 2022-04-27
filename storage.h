#include <stdint.h>

typedef uint64_t message_id_t;
typedef char* channel_id_t;

typedef struct message {
	message_id_t id;
	const char *text;
	struct message *next;
} message_t;


typedef struct channel {
	channel_id_t name;
	struct channel *next;
	message_t *head;
	message_t *tail;
	message_id_t last_msg;
} channel_t;

typedef struct channel_list {
	channel_t *head;
	channel_t *tail;
} channel_list_t;


// returns a channel_list_t that contains the head and tail of the list storing the channels
channel_list_t *get_channels();

// creates a channel whose name is given by channel name. 
// if a channel with that name already exists, a pointer to it is returned
channel_t *create_channel(channel_list_t *channels, const char *name);

// retrieves a channel by name.
// if a channel with that name does not exist, NULL is returned.
channel_t *get_channel(channel_list_t *channels, const char *name);

// frees all the channels and their messages
void free_channels(channel_list_t *channels);

// create a message_t that contains the provided text as the text field and adds it to channel/
// IMPORTANT: you must efforce the following invariats:
// 1) for a given channel, two messages cannnot have the same id
// 2) message_ids are allocated incrementally starting from 0
// 3) the biggest message id is one less than the value of last_msg of the channel
// 
void add_message(channel_t *channel, const char *text);

// searches the messages of channel and if it finds one that has the supplied id, the it is returned.
// If no matching message is found, then NULL is returned.
message_t *get_message(channel_t *channel, message_id_t id);


// prints the channels and the message
void dump(channel_list_t *channels);




