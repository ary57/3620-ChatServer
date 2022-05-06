#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "commands.h"

#define PORT 8080


// retrieve the message from the channel with name equal to channel name
// returns 0 if channel and message were found, otherwise -1.
int retrieve_message(int socket, const char *channel_name, message_id_t msg_id) {
	return 0;
}

void retrieve_messages(int socket, const char *channel_name) {
}

void send_message(int socket, const char *channel, char *text) {
}

void help(char **argv) {
	printf("%s -channel <name of channel> -text <text>\n", argv[0]);
	printf("\t\tsends the message on the channel\n\n\n");

	printf("%s -channel <name of channel> -msg <message id>\n", argv[0]);
	printf("\t\tretrieve the message with a given message id\n\n\n");

	printf("%s -channel <name of channel>\n", argv[0]);
	printf("\t\tretrieve all the messages from a given channel\n\n\n");
	exit(0);
}

int parse_args(int socket, int argc, char **argv) {
	if (argc == 1) help(argv); 

	const char *channel;
	if ((strcmp(argv[1], "-channel") != 0) || (argc <= 2)) {
		printf("Expected the two argument to specify the channel\n");	
		return -1;
	}
	channel = argv[2];
	
	printf("channel %s\n", channel);
	if (argc == 3) {
		retrieve_messages(socket, channel);
		return 0;
	}

	if (strcmp(argv[3], "-msg") == 0) {
		if (argc != 5) {
			help(argv);
			return -1;
		}
		
		int msg_id = atoi(argv[4]);
		printf("msg_id %d\n", msg_id);
		retrieve_message(socket, channel, msg_id);
		return 0;
	}

	if (strcmp(argv[3], "-text") == 0) {
		if (argc != 5) {
			help(argv);
			return -1;
		}
		
		char *text = argv[4];
		printf("text %s\n",text);
		send_message(socket, channel, text);
		return 0;
	}

	help(argv);
	return -1;
}

int main(int argc, char** argv) {
	printf("args=%d\n", argc);


	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	} else {
		printf("Socket successfully created..\n");
	}
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}

	parse_args(sockfd, argc, argv);

	// close the socket
	close(sockfd);
}
