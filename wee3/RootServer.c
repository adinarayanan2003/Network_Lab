

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9021
#define PORT1 9022

int main() {

	printf("Root Server\n");
	// creating socket
	int serverSocket, tldSocket, tldSocket1;
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	tldSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(serverSocket < 0) {
		perror("\nSocket failed\n");
		exit(EXIT_FAILURE);
	}
	
	// defining address
	struct sockaddr_in serverAddress, clientAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	// binding socket to address
	int checkBind = bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
	
	if(checkBind < 0) {
		perror("\nBind failed\n");
		exit(EXIT_FAILURE);
	}
	
	// defining tld addresses
	struct sockaddr_in tldAddress;
	memset(&serverAddress, 0, sizeof(tldAddress));
	
	tldAddress.sin_family = AF_INET;
	tldAddress.sin_port = htons(PORT1);
	tldAddress.sin_addr.s_addr = INADDR_ANY;
	
	int len = sizeof(clientAddress);
	
	while(1) {
		char str[200];
		int n = recvfrom(serverSocket, (char*) str, sizeof(str), MSG_WAITALL,
						 (struct sockaddr*) &clientAddress, &len);
		
		int length = strlen(str);
		
		printf("\nString that is sent by the local Server: %s\n", str);
		
		char tld[4];
		tld[0] = str[length-3];
		tld[1] = str[length-2];
		tld[2] = str[length-1];
		
		char ip[200];
		if(strcmp(tld, "com") == 0) {
			sendto(tldSocket, (char*) str, sizeof(str), MSG_CONFIRM, 
								(struct sockaddr*) &tldAddress, sizeof(tldAddress));
			int tldLen = sizeof(tldAddress);
			int x = recvfrom(tldSocket, (char*) ip, sizeof(ip), MSG_WAITALL,
								 (struct sockaddr*) &tldAddress, &tldLen);
			printf("TLD sent -> ");
		} else {
			strcpy(ip, "TOP LEVEL DOMAIN NOT FOUND!");
		}
		printf("%s\n", ip);
		
		sendto(serverSocket, (char*) ip, sizeof(ip), MSG_CONFIRM, 
				(struct sockaddr*) &clientAddress, sizeof(clientAddress));
		memset(&ip, 0, sizeof(ip));
		memset(&str, 0, sizeof(str));
	}
	
	close(serverSocket);
	close(tldSocket);
	return 0;
}
