# Client_Server_Tic_Tac_Toe

This repository consists of a Client, Server and Shared Program. The purpose of this program is to handle multiple clients concurrently with a single Server instance to play the game tic-tac-toe, mutually exclusively, with each client connection


Further Description of each program:

Server:
The server you create is to be a concurrent server.  After the server process executes a fork system call to create a child process to handle a particular client connection, have the child process exec another program, ServerG, to perform the server tasks for playing the game.  That is, the server will consist for two programs:  ServerC and ServerG. ServerC is responsible for accepting the initial connection and establishing each child process based on the ServerG program.  ServerC must pass the socket descriptor to ServerG.  In addition, ServerC must continue to accept connections and monitor the termination of all child processes.  The textbook:  TCP/IP Sockets in C by Donahoo and Calvert in Section 6.4.1 contains relevant ideas.  Appropriate console messages must be issued by ServerC to document the occurrence of significant events (new connection accepted, child created, child died, etc.).


Client:
The client is responsible for all communication with the human user who is playing the game.  This includes such things as prompting for user input, displaying the status of the game, displaying the server’s
“move”, displaying error messages, etc.


Shared code:
Create a C/C++-module that contains any common subroutines that are used by both the client and server.  Link the resulting code into both the server and client programs.  Client and server must utilize these routines.  In addition, if there are any shared structure definitions or symbol definitions, define these in a .h file that both client and server use.

