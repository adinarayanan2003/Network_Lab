
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9020
#define PORT1 9021

typedef struct ARecord {
	char domainName[200];
	char domainIP[200];
}ARecord;

int main() {

	printf("Local Server\n");
	
	ARecord* record[100];
	
	for(int i = 0; i < 100; i++) {
		record[i] = NULL;
	}
	// creating socket
	int serverSocket, rootSocket;
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	rootSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(serverSocket < 0) {
		perror("\nSocket failed\n");
		exit(EXIT_FAILURE);
	}
	
	// defining address
	struct sockaddr_in serverAddress, clientAddress, rootAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	memset(&rootAddress, 0, sizeof(rootAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	// binding socket to address
	int checkBind = bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
	
	if(checkBind < 0) {
		perror("\nBind failed\n");
		exit(EXIT_FAILURE);
	}
	
	rootAddress.sin_family = AF_INET;
	rootAddress.sin_port = htons(PORT1);
	rootAddress.sin_addr.s_addr = INADDR_ANY;
	
	int len = sizeof(clientAddress);
	
	
	while(1) {
		char str[200];
		int n = recvfrom(serverSocket, (char*) str, sizeof(str), MSG_WAITALL,
						 (struct sockaddr*) &clientAddress, &len);

		int length = strlen(str);
		printf("\nString that is sent by the client: %s\n", str);
		
		bool flag = true;
		
		for(int i = 0; i < 100; i++) {
			if(record[i] != NULL) {
				if(strcmp(record[i]->domainName, str) == 0) {
					sendto(serverSocket, (char*) record[i]->domainIP, sizeof(record[i]->domainIP), MSG_CONFIRM, 
									(struct sockaddr*) &clientAddress, sizeof(clientAddress));
					flag = false;
					break;
				}
			}
		}
			
		if(flag) {
			sendto(rootSocket, (char*) str, sizeof(str), MSG_CONFIRM, 
					(struct sockaddr*) &rootAddress, sizeof(rootAddress));
			char ip[200];
			int rootLen = sizeof(rootAddress);
			int x = recvfrom(rootSocket, (char*) ip, sizeof(ip), MSG_WAITALL,
						 (struct sockaddr*) &rootAddress, &rootLen);
			
			for(int i = 0; i < 100; i++) {
				if(record[i] == NULL) {
					record[i] = (ARecord *)malloc(sizeof(ARecord));
					strcpy(record[i]->domainName, str);
					strcpy(record[i]->domainIP, ip);
					break;
				}
			}
			
			printf("Root sent -> %s\n", ip);
			sendto(serverSocket, (char*) ip, sizeof(ip), MSG_CONFIRM, 
									(struct sockaddr*) &clientAddress, sizeof(clientAddress));
			memset(&ip, 0, sizeof(ip));
		}
		memset(&str, 0, sizeof(str));
	}
	
	close(serverSocket);
	close(rootSocket);
	return 0;
}
