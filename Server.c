/**
Description: Project 3 for CSCI 311.
Servers, Sockets and Tic-Tac-Toe

File:  ServerG.c
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
#define PREFIX "\e[0;33m""Client %d|""\e[0m "

int main(int argc, const char * argv[]) {
    
    // Variable Declaration
    int sockfd, ret, count = 0, move;
    struct sockaddr_in serverAddr;
    
    int newSocket;
    struct sockaddr_in newAddr;
    
    socklen_t addr_size;
    
    char buffer[1024];
    pid_t childpid;
    
    
    //Fork function here to handle multiple clients
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        printf("Error In Connection\n");
        exit(1);
    }
    
    printf("Server Socket Is Created\n");
    
    memset(&serverAddr, '\0', sizeof(serverAddr));
    
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    
    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0) {
        printf("Error In Binding\n");
        exit(2);
    }
    printf("Bind to port %d\n", PORT);
    
    if(listen(sockfd, 10) == 0) {
        printf("Listening.....\n");
    }
    else {
        printf("Error In Listening\n");
    }
    
    
    //infinite while loop
    while(1) {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if (newSocket < 0) {
            exit(3);
        }
        
        
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        
        if((childpid = fork()) == 0) {
            close(sockfd);
            int id = getpid();

            printf(PREFIX"Game started!\n", id);
            initialize_Board();
            int startPlayer = starting_Player();
            printf(PREFIX"The client is player 1. The computer is player 0.\n", id);
            printf(PREFIX"The starting player will be randomly chosen. Player %d will start the game.\n", id, startPlayer);

            int turnCount = startPlayer;
            int serverMove, playerMove;
            int gameOver = 0;
            char gameBuffer[1000];
            int input, recv_input;
            char inputBuffer[2];

            while(gameOver == 0) {

                if (startPlayer == 0) {
                    printf(PREFIX"Server is making the first move.\n", id);
                    memset(&inputBuffer, 0, sizeof(inputBuffer));
                    recv_input = recv(newSocket, &inputBuffer, sizeof(inputBuffer), 0);
                        if (recv_input < 0) {
                            printf(PREFIX"Input from client not recieved.\n",id);
                            exit(5);
                        } 
                    serverMove = getServerMove();
                    if(serverMove == 1) { //Sucessful move
                        printf(PREFIX"Server's move was successful.\n", id);
                        strcpy(gameBuffer, "Server has made a move.\n");
                        turnCount = 1;
                        startPlayer = 2;
                        strcat(gameBuffer, getBoardString(board));
                    } 
                    send(newSocket, gameBuffer, strlen(gameBuffer), 0);
                    bzero(gameBuffer, sizeof(gameBuffer));                 
                }

                else if (startPlayer == 1) {
                    playerMove = getPlayerMove(input);
                    if(playerMove == 1) { //Sucessful move
                        turnCount = 0;
                        startPlayer = 2;
                        printf(PREFIX"Board Updated!\n",id);
                        strcpy(gameBuffer, "Move sucessful\n");
                        strcat(gameBuffer, getBoardString(board));
                    }                    
                }
                

                if (turnCount == 0) {

                    printf(PREFIX"Server is making a move.\n", id);
                    serverMove = getServerMove();
                    //printf(PREFIX"serverMove = %d\n", id, serverMove);
                    if(serverMove == 1) { //Sucessful move
                        printf(PREFIX"Server's move was successful.\n", id);
                        strcat(gameBuffer, "Server has made a move.\n");
                        turnCount = 1;
                        strcat(gameBuffer, getBoardString(board));
                        strcat(gameBuffer, "It is now your turn. Please input your move:\n");
                    }
                    else if(serverMove == 0) {
                        printf(PREFIX"Server tried to move, but the game is tied.\n", id);
                        strcat(gameBuffer, "Server was unable to move, game is tied.\n");
                        strcat(gameBuffer, getBoardString(board));
                        gameOver = 1;
                    }
                    else if(serverMove == 2) {
                        printf(PREFIX"Server has won the game.\n", id);
                        strcat(gameBuffer, "Server has won.\n");
                        strcat(gameBuffer, getBoardString(board));
                        gameOver = 1;
                    }
                    printf(PREFIX"Server turn has ended.\n", id);
                    send(newSocket, gameBuffer, strlen(gameBuffer), 0);
                    bzero(gameBuffer, sizeof(gameBuffer));

                } //End of Server's turn

                // Player's Turn
                if (turnCount == 1) {
                    printf(PREFIX"It is the client's turn! They will make a move now.\n", id);
                    memset(&inputBuffer, 0, sizeof(inputBuffer));
                    recv_input = recv(newSocket, &inputBuffer, sizeof(inputBuffer), 0);
                        if (recv_input < 0) {
                            printf(PREFIX"Input from client not recieved.\n", id);
                            exit(5);
                        } 
                    input = atoi(&inputBuffer[0]);

                    playerMove = getPlayerMove(input);
                    //printf(PREFIX"playerMove = %d\n", id, playerMove);
                    if(playerMove == 1) { //Sucessful move
                        turnCount = 0;
                        printf(PREFIX"Board Updated!\n", id);
                        strcat(gameBuffer, "Move sucessful\n");
                        strcat(gameBuffer, getBoardString(board));
                    }
                    else if(playerMove == 3) { //Game has tied
                        printf(PREFIX"Player has tied the game.\n", id);
                        gameOver = 1;
                        strcat(gameBuffer, "The game is tied.\n");
                        strcat(gameBuffer, getBoardString(board));
                    }
                    else if(playerMove == 0) { //Invalid move/input
                        strcpy(gameBuffer, "The selected position is either taken or invalid.\n");
                        send(newSocket, gameBuffer, strlen(gameBuffer), 0);
                        bzero(gameBuffer, sizeof(gameBuffer));
                    }
                    else if(playerMove == 2) { //Player has won.
                        printf(PREFIX"Player has won the game.\n", id);
                        strcat(gameBuffer, "Player has won!\n");
                        strcat(gameBuffer, getBoardString(board));
                        gameOver = 1;
                    }
                } //End of Player's turn

                if(gameOver == 1) { //If the game is over, send a final response.
                    printf(PREFIX"%s", id, getBoardString(board));
                    strcat(gameBuffer, "The game is now over. Entering echo loop. Type :exit to close the client.\n");
                    send(newSocket, gameBuffer, strlen(gameBuffer), 0);
                }
            }
            
            while(1) {
                bzero(buffer, sizeof(buffer));
                recv(newSocket, buffer, 1024, 0);
                if(strcmp(buffer, ":exit") == 0) {
                    printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
                    break;
                }
                else {
                    printf("Client: %s\n", buffer);
                    send(newSocket, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                    
                }
            }
        }
    }
    
    close(newSocket);
    
}

