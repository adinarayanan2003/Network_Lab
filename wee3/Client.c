

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9020

int main() {

	printf("Client side(UDP)\n");
	// creating client socket
	int clientSocket;
	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(clientSocket < 0) {
		perror("\nSocket failed\n");
		exit(EXIT_FAILURE);
	}
	
	// defining address
	
	struct sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	while(1) {
		char str[200];
		printf("\nEnter the input string: ");
		fgets(str, 200, stdin);
		
		int len = strlen(str);
		str[len - 1] = '\0';
		
		
		sendto(clientSocket, (char*) str, sizeof(str), MSG_CONFIRM, 
				(struct sockaddr*) &serverAddress, sizeof(serverAddress));
		printf("\nString sent to server\n");
		
		int l = sizeof(serverAddress);
		char ip[200];
		int n = recvfrom(clientSocket, (char*) ip, sizeof(ip), MSG_WAITALL,
						  (struct sockaddr*) &serverAddress, &l);
		
		printf("%s IP -> %s\n", str, ip);
		memset(&ip, 0, sizeof(ip));
		memset(&str, 0, sizeof(str));
	}
	
	
	close(clientSocket);
	return 0;
}
