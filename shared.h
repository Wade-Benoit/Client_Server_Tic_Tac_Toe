/**
Description: Project 3 for CSCI 311.
Servers, Sockets and Tic-Tac-Toe

File: shared.h
Authors: Sameep Koirala (StarId: xs9942bz), Wade Benoit (StarID: EU2401OT), Skylar Wolf (StarID: gv4316no), Melissa Murray (StarID: qc5533d)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#define  CLIENT 1
#define  SERVER 2

int board[3][3];
int player = -1;
int tie = 0;
int win = 0;

/** @brief  starting_Player
* The initial player is randomized
* @pre None
* @post Returns a randomized 1 or 0 to initialize
*	    the starting player.
*/
int starting_Player() {
    srand(time(NULL));
    player = rand() % 2; // client = 1 and server = 0
    return player;
}

/** @brief  initialize_Board
* The tic tac toe board is created
* @pre None
* @post Returns a double character array to simulate
*	    the tic tac toe board.
*/
void initialize_Board()
{
    int i, j;
    for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
            board[i][j] = 0;	
} 

/** @brief  check_win
* A single move is checked against winning scenarios on the board
* @pre int input: A valid move is made by the player on the board
* @post Returns 1 if the input resulted in a win and 2 if the 
*	    move did not result in a win.
*/
int check_win(int input)
{
    //tie = check_tied();
	
	if(input == 0)
		return 0; //Case in which the user tried to check if a blank space won
	//else if (tie == 1)
		//return 2; // case in which the game is tied and not won
    else if(board[0][0] == input && board[0][1] == input && board[0][2] == input)
        return 1; // case in which the game is won
    else if(board[0][0] == input && board[1][1] == input && board[2][2] == input)
        return 1; // case in which the game is won
    else if(board[1][0] == input && board[1][1] == input && board[1][2] == input)
        return 1; // case in which the game is won
    else if(board[2][0] == input && board[2][1] == input && board[2][2] == input)
        return 1; // case in which the game is won
    else if(board[0][1] == input && board[1][1] == input && board[2][1] == input)
        return 1; // case in which the game is won
    else if(board[0][2] == input && board[1][2] == input && board[2][2] == input)
        return 1; // case in which the game is won
    else if(board[0][0] == input && board[1][0] == input && board[2][0] == input)
        return 1; // case in which the game is won
    else if(board[0][2] == input && board[1][1] == input && board[2][0] == input)
        return 1; // case in which the game is won

    return 0; // case in which the game continues
}

/** @brief  check_tied
* The board is checked for no remaining spaces and no winning scenarios
* @pre None
* @post Returns 1 if a tie condition is found on the board, and returns
*	    0 if open space remains on the board.
*/
int check_tied()
{
	int i, j;
	int chkWin1 = check_win(1);
	int chkWin2 = check_win(2);
	if(chkWin1 == 1 || chkWin2 == 1) {
		//printf("Tie check: 0\n");
		return 0;
	}
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            //if(board[i][j] != 1 && board[i][j] != 2)
			if(board[i][j] == 0) {
				//printf("Tie check: 0\n");
                return 0;
			}
        }
    }
	//printf("Tie check: 1\n");
    return 1;
}

/** @brief  getBoardString
* The tic tac toe board is printed with player moves
* @pre A valid two dimensional character array containing 1 and 2
* @post Returns a character string printing the tic tac toe board
*	    with the player moves filled respectively with X and O
*/
char* getBoardString(int arr[3][3]) {
	static char buffer[1000];
	char buffer2[1000];
    int i,j;
    char printBoard [3][3][1] =  {{'_','_','_'},{'_','_','_'},{'_','_','_'}};

    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if(arr[i][j] == 1) { //If player claimed position
                printBoard[i][j][0] = 'X';
            }
            else if(arr[i][j] == 2) { //If server claimed position
                printBoard[i][j][0] = 'O';
            }
			else if(arr[i][j] > 2 || arr[i][j] < 0) { //Error checking
				printf("WARNING: Board position (%d, %d) is invalid.", i, j);
			}
        }
    }

	//buffer[0]='\0'; //Buffer seems to persist, so it must be cleared each time this function runs.
	bzero(buffer, sizeof(buffer));
	strcat(buffer, "Here is the current board:\n");
	sprintf(buffer2, "%c,%c,%c\n",printBoard[0][0][0],printBoard[0][1][0],printBoard[0][2][0]); //Sets buffer 2 to the values of the current row
	strcat(buffer, buffer2); //Adds the string from buffer 2 to buffer
	sprintf(buffer2, "%c,%c,%c\n",printBoard[1][0][0],printBoard[1][1][0],printBoard[1][2][0]); //Sets buffer 2 to the values of the current row
	strcat(buffer, buffer2); //Adds the string from buffer 2 to buffer
	sprintf(buffer2, "%c,%c,%c\n",printBoard[2][0][0],printBoard[2][1][0],printBoard[2][2][0]); //Sets buffer 2 to the values of the current row
	strcat(buffer, buffer2); //Adds the string from buffer 2 to buffer
	//printf("Showing the player the current board.\n----------------------------------------");
	//printf(buffer); //If the server needs to see it.
	return buffer;
} 

/** @brief  getPlayerMove
* The players choice on the board is played on the tic tac toe board
* @pre int move
* @post The int move is checked for validity and place on the board
*	    if available. The board is checked for win (return 1) or tie 
*		(return 3) scenarios. If the games continues, return 1. Else
*		return 0.
*/
int getPlayerMove(int move)
{
    int x, y;

	if(move > 0 && move < 4) { //If the move is 1, 2, or 3.
		x = 0;
		y = move - 1;
	}
	else if(move > 3 && move < 7) { //If the move is 4, 5, 6
		x = 1;
		y = move - 4;
	}
	else if(move > 6 && move < 10) { //If the move is 7, 8, 9
		x = 2;
		y = move - 7;
	}
	else { //If the move is invalid.
		return 0;
	}

	//printf("Player is attempting a move at X: %d, Y: %d\n", x, y); //Debug
		
		if (board[x][y] == 0)
		{
			board[x][y] = 1;
			
			win = check_win(1);
			tie = check_tied();
			
				if (win == 1) {
					// case in which the player won
					return 2;
				}
				if (tie == 1){
					// case in which the game has tied
					return 3;
				}
				
			// else the game continues and a move has been sucessfully made
			return 1;
		}	
		else
		{
			// case in which the space is not available			
			return 0;
		}	
}

/** @brief  getServerMove
* A random integer between 1 and 9 is played on the tic tac toe board
* @pre None
* @post Returns a random int between 1 and 9. Previously played moves are
*	    stored in an array for reference. Each successfull input is checked 
*		for a win or tie scenario.
*/
int getServerMove() {
	int move, avail = 0;
	int x,y, i, j;
	int possibleMoves = 0;
	int positionCount = 1;
	int potentialMoves[9];

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			if(board[i][j] == 0) {
				potentialMoves[possibleMoves] = positionCount;
				//printf("Found available spot at %d\n", positionCount);
				possibleMoves++;
			}
			positionCount++;
		}
	}
		

	while(avail == 0) {
		srand(time(NULL));
		//move = (rand() % 9) + 1; // random number between 1 and 9
		move = (potentialMoves[(rand()%possibleMoves)]);
	
		x = (move - 1) / 3;
		y = (move - 1) % 3;
	
		if (board[x][y] == 0)
		{
			board[x][y] = 2;
			avail = 1;
			
			win = check_win(2);			
			tie = check_tied();
		
			if (win == 1) {
			//case in which the server won
			return 2;
			}

			if (tie == 1) {
			// Case in which the server can't make a move -- game is tied.
			return 0;
			}
			
		// case in which the server has made a move and the game continues		
		return 1;
		}
	}
}
