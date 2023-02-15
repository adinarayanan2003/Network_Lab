
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9023

typedef struct Arecord {
	char domainName[200];
	char domainIP[200];
} Arecord;

int main() {

	printf("Auth Server\n");
	// creating socket
	
	Arecord rec[2];
	
	strcpy(rec[0].domainName, "www.pqr.com");
	strcpy(rec[0].domainIP, "192.168.5.60");
	
	strcpy(rec[1].domainName, "www.xyz.com");
	strcpy(rec[1].domainIP, "192.8.1.100");
	
	int serverSocket;
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
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

	
	int len = sizeof(clientAddress);
	
	while(1) {
		char str[200];
		int n = recvfrom(serverSocket, (char*) str, sizeof(str), MSG_WAITALL,
						 (struct sockaddr*) &clientAddress, &len);
		
		int length = strlen(str);
		int left = 0, right = length - 1;
		printf("\nString that is sent by the TLD Server: %s\n", str);
		
		bool flag = true;
		for(int i = 0; i < 2; i++) {
			if(strcmp(rec[i].domainName, str) == 0) {
				sendto(serverSocket, (char*) rec[i].domainIP, sizeof(rec[i].domainIP), MSG_CONFIRM, 
						(struct sockaddr*) &clientAddress, sizeof(clientAddress));
				flag = false;
				break;
			}
		}
		
		if(flag) {
			char* s = "NOT FOUND! Enter correct Domain Name";
			sendto(serverSocket, (char*) s, sizeof(s), MSG_CONFIRM, 
						(struct sockaddr*) &clientAddress, sizeof(clientAddress));
		}

		memset(&str, 0, sizeof(str));
	}
	
	close(serverSocket);
	return 0;
}
