// Server side 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
	
#define PORT	 9006
#define MAXLINE 1024
#define MSG_CONFIRM 0
// Driver code
int main() {
	int sockfd;
	char buffer[MAXLINE];
	char *hello = "Hello from server";
	struct sockaddr_in servaddr, cliaddr;
		
	// Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
		
	memset(&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));
		
	// Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT);
		
	// Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
		
	int len, n;
	
	len = sizeof(cliaddr); //len is value/result
	
	while(1){
	 bzero(buffer, 1024);
	n = recvfrom(sockfd, (char *)buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr *) &cliaddr,
				&len);
	buffer[n] = '\0';
	printf("Client message : %s\n", buffer);

     int len1 = strlen(buffer)-1,i;

    for(int i=0;i<len1/2;i++)
    {
        char temp = buffer[i];
        buffer[i]=buffer[len1-1-i];
        buffer[len1-1-i]=temp;

    }
    printf("Server response: %s\n", buffer);

	sendto(sockfd, (const char *)buffer, strlen(buffer),
		MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
			len);
	printf("message sent.\n");
	}
		
	return 0;
}
