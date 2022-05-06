#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <assert.h>

#include "commands.h"

#define PORT 8080



void run_server(int sockfd, int num_threads) {
    struct sockaddr_in cli;
    socklen_t len = sizeof(cli);

    while(1) {
        // Accept the data packet from client and verification
        int client_socket = accept(sockfd, (struct sockaddr*)&cli, &len);
        if (client_socket < 0) {
            printf("server accept failed...\n");
            exit(0);
        } else {
            printf("server accept the client...\n");

			// implement the communication with the client
        }
    }
}




int main() {
	channel_list_t *channels = get_channels();
	channel_t *weather = create_channel(channels, "weather");
	channel_t *new = create_channel(channels, "news");

	add_message(weather, "the weather is going to be great!");
	add_message(weather, "sunny with a high of 70F");
	dump(channels);


	int sockfd;
	struct sockaddr_in servaddr;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	} else {
		printf("Socket successfully created..\n");
    }
    
	// assign IP, PORT
	bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	} else {
		printf("Socket successfully binded..\n");
    }

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	} else {
		printf("Server listening..\n");
    }
	
    run_server(sockfd, 10);



	// After chatting close the socket
	close(sockfd);
}
