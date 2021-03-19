/**
Description: Project 3 for CSCI 311.
Servers, Sockets and Tic-Tac-Toe

File: ServerC.c
Authors: Sameep Koirala (StarId: xs9942bz), Wade Benoit (StarID: EU2401OT), Skylar Wolf (StarID: gv4316no), Melissa Murray (StarID: qc5533d)
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "shared.h"

#define PORT 8888


int main(int argc, const char * argv[]) {
    
    
    int clientSocket, ret, startingPlayer;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    
    if(clientSocket < 0) {
        printf("Error In Connection\n");
        exit(0);
    }
    
    printf("Client Socket Is Created\n");
    printf("Please Enter an address to connect to (127.0.0.1 for local): ");
    scanf("%s", &buffer[0]);
    
    memset(&serverAddr, '\0', sizeof(serverAddr));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    //serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_addr.s_addr = inet_addr(buffer);
    
    
    
    ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0) {
        printf("Error In Connection\n");
        exit(1);
    }
    printf("Connected To Server\n");
    printf("Please input something to start the game.\n");

    /*printf("Game started!\n");
    printf("You are player 1. The computer is player 2.\n");
    printf("The starting player will be randomly chosen. Player %d will start the game.\n", startPlayer);*/
    
    
    bzero(buffer, sizeof(buffer));
    //Infinite Loop for client to communicate with serverG until completion
    while(1) {

        printf("Client: \t");
        scanf("%s", &buffer[0]);
        send(clientSocket, buffer, strlen(buffer), 0);
        
        if(strcmp(buffer, ":exit") == 0) {
            printf("Disconnected From Server.\n");
            close(clientSocket);
        }

        if(recv(clientSocket, buffer, 1024, 0) < 0) {
            printf("Error In Receiving data\n");
            exit(1);
        }
        else {
            //What the server says in response from buffer
            printf("Server:  \t%s\n", buffer);
        }
        bzero(buffer, sizeof(buffer));
        
    }
    
    
    return 0;
}




