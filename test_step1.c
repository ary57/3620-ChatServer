#include <stdio.h>
#include <stdlib.h>
#include "storage.h"

int main() {
    channel_list_t *channels = get_channels();
	channels->head = channels->tail = NULL;
	channel_t *weather = create_channel(channels, "weather");
	create_channel(channels, "news");
    // create_channel(channels, "the_empty_channel");

	add_message(weather, "the weather is going to be great!");
	add_message(weather, "sunny with a high of 70F");

    channel_t *news = get_channel(get_channels(), "news");
    add_message(news, "summer vacation is almost here");
    add_message(news, "yay!");
    add_message(news, ":-)");
	dump(channels);

    free_channels(get_channels());
    
    printf("AFTER THE FIRST FREE\n");
    if(channels == NULL) printf("CHANNEL IS NULL\n");
    else printf("CHANNEL IS NOT NULL \n");

    // printf("channels->head->name: %s\n",channels->head->name);
    
    dump(channels);

    return 0;
}