

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 9022
#define PORT1 9023

typedef struct NSrecord {
	char authName[200];
	int authSocket;
} NSrecord;

int main() {

	printf("TLD Server(.com)\n");
	// creating socket
	
	NSrecord rec[2];
	
	int serverSocket;
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	if(serverSocket < 0) {
		perror("\nSocket failed\n");
		exit(EXIT_FAILURE);
	}
	
	strcpy(rec[0].authName, "pqr");
	rec[0].authSocket = socket(AF_INET, SOCK_DGRAM, 0);
	strcpy(rec[1].authName, "xyz");
	rec[1].authSocket = socket(AF_INET, SOCK_DGRAM, 0);
	
	// defining address
	
	struct sockaddr_in serverAddress, clientAddress, senderAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	memset(&clientAddress, 0, sizeof(clientAddress));
	memset(&senderAddress, 0, sizeof(senderAddress));
	
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	
	// binding socket to address
	int checkBind = bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress));
	
	if(checkBind < 0) {
		perror("\nBind failed\n");
		exit(EXIT_FAILURE);
	}
	
	
	// authoritative server binding
	struct sockaddr_in auth1;
	memset(&auth1, 0, sizeof(auth1));
	
	auth1.sin_family = AF_INET;
	auth1.sin_port = htons(PORT1);
	auth1.sin_addr.s_addr = INADDR_ANY;
	
	
	int len = sizeof(clientAddress);
	
	
	while(1) {
		char str[200];
		int n = recvfrom(serverSocket, (char*) str, sizeof(str), MSG_WAITALL,
						 (struct sockaddr*) &clientAddress, &len);
		str[n] = '\0';
		
		int length = strlen(str);
		int left = 0, right = length - 1;
		printf("\nString that is sent by the Root Server: %s\n", str);
		
		while(str[left] != '.')
			left++;
		while(str[right] != '.')
			right--;
		
		char domain[10];
		
		left++;
		int i = 0;
		while(left < right) {
			domain[i] = str[left];
			left++;
			i++;
		}
		
		char ip[200];
		int l = sizeof(senderAddress);
		
		if(strcmp(rec[0].authName, domain) == 0) {
			
			sendto(rec[0].authSocket, (char*) str, sizeof(str), MSG_CONFIRM, 
				(struct sockaddr*) &auth1, sizeof(auth1));
				
			int x = recvfrom(rec[0].authSocket, (char*) ip, sizeof(ip), MSG_WAITALL,
						 (struct sockaddr*) &senderAddress, &l);
			printf("Auth Send -> ");
			
		} else if(strcmp(rec[1].authName, domain) == 0) {
		
			sendto(rec[1].authSocket, (char*) str, sizeof(str), MSG_CONFIRM, 
				(struct sockaddr*) &auth1, sizeof(auth1));
			
			int x = recvfrom(rec[1].authSocket, (char*) ip, sizeof(ip), MSG_WAITALL,
						 (struct sockaddr*) &senderAddress, &l);
			printf("Auth Send -> ");
		} else {
			strcpy(ip, "NOT FOUND! Enter correct Domain Name");
		}
		printf("%s\n", ip);
		
		sendto(serverSocket, (char*) ip, sizeof(ip), MSG_CONFIRM, 
				(struct sockaddr*) &clientAddress, sizeof(clientAddress));
		memset(&ip, 0, sizeof(ip));
		memset(&str, 0, sizeof(str));
	}
	
	
	close(serverSocket);
	close(rec[0].authSocket);
	close(rec[1].authSocket);
	return 0;
}
