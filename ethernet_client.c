#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

#define PORT 3030
#define BUF 1024

int main(int argc, char *argv[]){
    int sock = 0;
	struct sockaddr_in remote;
	int dataBytes = 0;
	int sent = 0;
	char ip[INET_ADDRSTRLEN] = {0};
	char *data = NULL;
	char *buffer[BUF] = {0};

	/* Get IP */
	if (argc > 1){
		strcpy(ip, argv[1]);
	}
	else{
		printf("Usage: %s <server ip address>\n",argv[0]);
		return -1;
	}

	sock = socket(AF_INET , SOCK_STREAM , IPPROTO_TCP);
	if(sock < 0){
		perror("socket");
		return -1;
	}
	remote.sin_family = AF_INET;
	remote.sin_addr.s_addr = INADDR_ANY;
	dataBytes = inet_pton(AF_INET, ip, (void *)(&(remote.sin_addr.s_addr)));
	if( dataBytes < 0 || dataBytes == 0){
		perror("inet_pton");
		return -1;
	}

	remote.sin_port = htons(PORT);

	if(connect(sock, (struct sockaddr *)&remote, sizeof(remote)) < 0){
		perror("connect");
		return -1;
	}
	else{
			printf("Connection was established\n");
	}
	/* Sending data */

	data = malloc(BUF);
	printf("Enter your message:");
	scanf(" %[^\n]", data);
	sent = 0;
	while(sent < strlen(data)){
		dataBytes = send(sock, data+sent, strlen(data)-sent, 0);
		if(dataBytes == -1){
			return -1;
		}
		sent += dataBytes;
		printf("Message length is: %d\n", sent);
	}
	free(data);
	data = NULL;
	/* Sending data END */

	/* Receiving data */
	printf("waiting for server to respond ....\n");
	dataBytes = 0;
	dataBytes = recv(sock, buffer, BUF,  0);
	if ( dataBytes > 0 ){
		printf("Response from server with ip %s is :\n%s\n", ip, buffer);
	}
	close(sock);

	return 1;
}
