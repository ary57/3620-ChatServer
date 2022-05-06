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

void retrive_message_resp(int client_socket){
	size_t channel_size;
	char *channel_name; 
	message_id_t msg_id;

	read(client_socket, &channel_size, sizeof(size_t));
	read(client_socket, channel_name, channel_size);
	read(client_socket, &msg_id, sizeof(msg_id));

	channel_list_t *channels = get_channels();
	
	pthread_mutex_lock(&channels->lock); 
	
	channel_t *channel = get_channel(channels, channel_name);

	if(channel == NULL){
		size_t errorSize = sizeof("!! Channel not found\n");
		char * error = (char *) malloc(errorSize);
		error = "!! Channel not found\n";
		write(client_socket, &errorSize, sizeof(errorSize));
		write(client_socket, error, errorSize);
		
		pthread_mutex_unlock(&channels->lock); 
		
		return;
	}
	
	message_t *msg = get_message(channel, msg_id);
	if(msg == NULL){
		size_t errorSize = sizeof("!! Message not found\n");
		char * error = (char *) malloc(errorSize);
		error = "!! Message not found\n";
		write(client_socket, &errorSize, sizeof(errorSize));
		write(client_socket, error, errorSize);
		
		pthread_mutex_unlock(&channels->lock); 
		
		return;
	}
	const char *txt = msg->text;

	size_t txt_len = strlen(txt);

	write(client_socket, &txt_len, sizeof(txt_len));
	write(client_socket, txt, txt_len);
	
	pthread_mutex_unlock(&channels->lock); 
}

void retrive_messages_resp(int client_socket){
	size_t channel_size;
	char *channel_name; 

	read(client_socket, &channel_size, sizeof(size_t));
	read(client_socket, channel_name, channel_size);

	channel_list_t *channels = get_channels();
	
	pthread_mutex_lock(&channels->lock); 
	
	channel_t *channel = get_channel(channels, channel_name);

	if(channel == NULL){
		size_t errorSize = sizeof("!! Channel not found\n");
		char * error = (char *) malloc(errorSize);
		error = "!! Channel not found\n";
		write(client_socket, &errorSize, sizeof(errorSize));
		write(client_socket, error, errorSize);

		pthread_mutex_unlock(&channels->lock); 

		return;
	}
	
	message_t *msg;
	message_id_t msg_id = 0;
	do{
		msg = get_message(channel, msg_id);
		if(msg == NULL){
			size_t errorSize = sizeof("!! Message not found\n");
			char * error = (char *) malloc(errorSize);
			error = "!! Message not found\n";
			write(client_socket, &errorSize, sizeof(errorSize));
			write(client_socket, error, errorSize);
			pthread_mutex_unlock(&channels->lock); 
			return;
		}
		const char *txt = msg->text;

		size_t txt_len = strlen(txt);

		write(client_socket, &txt_len, sizeof(txt_len));
		write(client_socket, txt, txt_len);
		msg_id++;
	}while(msg != NULL);
	pthread_mutex_unlock(&channels->lock); 
}

void send_message_resp(int client_socket){
	size_t channel_size;
	char *channel_name;
	size_t text_size; 
	char *text; 

	read(client_socket, &channel_size, sizeof(size_t));
	read(client_socket, channel_name, channel_size);

	read(client_socket, &text_size, sizeof(size_t));
	text = (char *) malloc(text_size);
	read(client_socket, text, text_size);

	channel_list_t *channels = get_channels();
	pthread_mutex_lock(&channels->lock); 
	channel_t *channel = get_channel(channels, channel_name);

	if(channel == NULL){
		channel = create_channel(channels, channel_name);
	}
	add_message(channel, text);
	pthread_mutex_unlock(&channels->lock); 

	// printf("message added: %s", text);
	
}

typedef struct args{
	int num_threads, socket;
}args_t; 

// void run_server(int sockfd, int num_threads) {
void run_server(void * args) {
	args_t arg = *(args_t *) args; 	
	int sockfd = arg.socket;
	int num_clients = arg.num_threads;

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
			/*
				USED FLAGS: 
				0 - retrieve message
				1 - retrieve messages
				2 - send messages
				3 - help???
			*/
			int flag; 
			read(client_socket, &flag, sizeof(int));

			switch(flag){
				case 0:
					retrive_message_resp(client_socket);
					break;
				case 1:
					retrive_messages_resp(client_socket); 
					break;
				case 2:
					send_message_resp(client_socket);
					break; 
				case 3:
					break;
			}
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
	
	int num_threads = 100; 

	pthread_t threads[num_threads];

	args_t args = *(args_t *) malloc(sizeof(args_t));
	args.socket = sockfd; 

    for (int i = 0; i < num_threads; i++) {
        int r = pthread_create(&threads[i], NULL, (void *)run_server, &args);
        if (r != 0) {
            printf("Failure to create thread\n");
            exit(-1);
        } 
    } 

	for (int i = 0; i < num_threads; i++) {
        int r = pthread_join(threads[i], NULL);
        if (r != 0) {
            printf("Failure to join on thread\n");
            exit(-1);
        }
    }
    // run_server(sockfd, 10);



	// After chatting close the socket
	close(sockfd);
}
