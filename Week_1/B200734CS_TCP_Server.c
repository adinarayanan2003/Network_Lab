#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    char server_message[256];

    //create the server socket 
    int server_socket,clientmsg,len;
    server_socket = socket(AF_INET, SOCK_STREAM, 0) ;

    // define the server address 
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(9002) ;
    server_address.sin_addr.s_addr = INADDR_ANY;

    // bind the socket to our specified IP and port
    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) ;
    listen(server_socket, 5);
    int client_socket;
    client_socket = accept (server_socket,(struct sockaddr*)&clientmsg, &len);

    char buff[80];
    bzero(buff, 80);

    read(client_socket, buff, sizeof(buff)); // copy msg to buffer array

    printf("From client To server :  %s\n", buff); //message from client
    
    int len1 = strlen(buff)-1,i;

    for(int i=0;i<len1/2;i++)
    {
        char temp = buff[i];
        buff[i]=buff[len1-1-i];
        buff[len1-1-i]=temp;

    }
    
    //sending back new buff aray
     printf("From server To client :  %s\n", buff);

    // send the message
    send(client_socket, buff, sizeof (buff), 0);
    // close the socket
    close(server_socket);

    return 0;
}