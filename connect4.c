// Author: Caleb Baptista

#include "connect4.h"

int PlayConnectFour(void);
int PlayOneGame(int startingPlayer, int size, int gameType);

int main(void)
{
	int userResponse = 1;
	while(userResponse){
		userResponse = PlayConnectFour();
	}
	return 0;
}

int PlayConnectFour(void)
{
	int size, gameType, numberOfGames, result, userResponse;
	int i, wins1, wins2, player;
	srand((unsigned int)time(NULL));

	// Prompt the user for the board size
	printf("\n\n==================");
	printf("\nEnter board size: ");
	scanf("%d", &size);

	// Get play options:
	printf("Options:\n");
	printf(" [1] = Human vs. Human\n");
	printf(" [2] = Human vs. Bot1\n");
	printf(" [3] = Bot1 vs. Bot2\n");
	printf("Choose game type: ");
	scanf("%d", &gameType);
	
	numberOfGames = 1;
	result = 0;
	
	// If two bots are playing a tournament, let the user choose how many games
	if (gameType == 3) {
		printf("Number of games: ");
		scanf("%d", &numberOfGames);
	}

	wins1 = 0;
	wins2 = 0;
	player = 1;

	for (i = 0; i < numberOfGames; i++) {
		result = PlayOneGame(player, size, gameType);
		if (result == 1) {
			wins1++;
		} else {
			wins2++;
		}
		// Switch the starting player for the next game
		player = 3 - player;
	}

	// If a single game was played, show the result of the game, otherwise report the result of the tournament:
	if (numberOfGames == 1) {
		printf("\nGame over! Congratulations! Winner is Player %d\n\n", result);
	} else {
		printf("\nTournament over! Games played = %d\nPlayer 1 wins = %d / Player 2 wins = %d\n\n", numberOfGames, wins1, wins2);
	}
	
	printf("Play again?:\n");
	printf(" [0] = No\n");
	printf(" [1] = Yes\n");
	scanf("%d", &userResponse);
	return userResponse;
	
}

void GetMoveHuman(char *side, int *move, int player)
{
	char a = ' ';
	char b = ' ';
	printf("Player %d: enter move [side/position]: ", player);
	while (!(a == 'N' || a == 'W' || a == 'E' || a == 'S')) {
		scanf("%c", &a);
	}
	while (!(b >= '0' && b <= '9')) {
		scanf("%c", &b);
	}
	*side = a;
	*move = (int)(b - '0');
}

int PlayOneGame(int startingPlayer, int size, int gameType)
{
	int board[MAX_SIZE][MAX_SIZE];
	char displayBoardString[(MAX_SIZE+5)*(MAX_SIZE+5)];
	int player, gameOver, move, lastRow, lastCol;
	char side;

	player = startingPlayer;
	gameOver = 0;

	// Initialise the board, and display it if a human is involved in this game
	InitialiseBoard(board, size);
	if (gameType != 3) {
		GetDisplayBoardString(board, size, displayBoardString);
		printf("%s", displayBoardString);
	}

	// Play one move at a time, displaying the board if necessary, until the game is over
	while (!gameOver) {
		if (gameType == 1) {
			GetMoveHuman(&side, &move, player);
		} else if (gameType == 2) {
			if (player == 1) {
				GetMoveHuman(&side, &move, player);
			} else {
				GetMoveBot1(board, size, player, &side, &move);
			}
		} else {
			if (player == 1) {
				GetMoveBot1(board, size, player, &side, &move);
			} else {
				GetMoveBot2(board, size, player, &side, &move);
			}
		}
		AddMoveToBoard(board, size, side, move, player, &lastRow, &lastCol);
		gameOver = CheckGameOver(board, size, player, lastRow, lastCol);
		if (gameType != 3) {
			GetDisplayBoardString(board, size, displayBoardString);
			printf("%s", displayBoardString);
		}
		if (!gameOver) {
			player = 3 - player;
		}
	}
	return player;
}

int FourInARow(int values[], int length)
{
	// Determine if a value is repeated four times in a row within a 1D array, if so return the index position 
	// of the start of this group
	int value;
	int check;
	
	// For each value in array check if it begins a pattern of four elements
	for (int i = 0; i < length; i++) {
		value = values[i];
		check = 1;

		//Check the next three elements in the array
		for (int j = 1; j < 4; j++) {
			if ((i + j) < length) { // Make sure index isn't out of array bounds
				// If any of the next three elements in the array is not the same as the value at the current index, 
				// the current index doesn't start a pattern of four
				if (values[(i + j)] != value) {
					check = 0;
				}
			}
			else {
				check = 0;
			}
		}

		if (check == 1) {
			return i; // If a pattern of four is found return the index of the first element in this pattern
		}
	}
	return -1;
}

void Swap(double *array_list, int x, int y) {
	// Swap values stored at two index positions 
	double temporary = array_list[x];
	array_list[x] = array_list[y];
	array_list[y] = temporary;
}

void Bubble(double *array_list, int length) {
	// If elements in adjacent index positions are out of order swap them using Swap function
	for (int i = 0; i < (length - 1); i++) {
		if (array_list[i] > array_list[i + 1]) {
			Swap(array_list, i, i + 1);
		}
	}
}

void Sort(double *array_list, int length) {
	for (int i = 0; i < length; i++) {
		Bubble(array_list, length);
	}
}

void InitialiseBoard(int board[MAX_SIZE][MAX_SIZE], int size)
{
	if ((size % 2) == 0) { // If board size is equal...
		// Loop through each space in board to find the four middle coordinates and set their value to 3. 
		// Set all other positions to 0
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if ((i == size / 2 || i == (size / 2) - 1) && (j == size / 2 || j == (size / 2) - 1)) {
					board[i][j] = 3;
				}
				else {
					board[i][j] = 0;
				}
			}
		}
	}
	else { // If board size is uneven
		// Loop through each space in board to find the middle coordinate and set its value to 3. 
		// Set all other positions to 0
		for (int i = 0; i < size; i++) { 
			for (int j = 0; j < size; j++) {
				if (i == size / 2 && j == size / 2) {
					board[i][j] = 3;
				}
				else {
					board[i][j] = 0;
				}
			}
		}
	}
}

void AddMoveToBoard(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move, int player, int *lastRow, int *lastCol)
{
	int i;
	int j;
	int moveStored = 0;
	
	// Depending on the side the player drops the counter into set either the row or coloumn to the move value and 
	// move the counter to further index positions until it reaches an obstacle 
	if (side == 'N') {
		j = move;
		for (i = 0; i < size; i++) {
			// If a position on the board which isn't vacant is reached then the counter is placed in the prior position 
			// if it isn't out of array bounds
			if (board[i][j] != 0) {
				if ((i - 1) >= 0) { 
					*lastRow = (i - 1);
					*lastCol = j;
					board[(i - 1)][j] = player;
				}
				// If there isn't a position on the board available for player counter indicate this by setting pointers to -1
				else {
					*lastRow = -1;
					*lastCol = -1;
				}
				moveStored = 1;
				break;
			}
		}
		// Store the counter at the side nearest to the side the counter entered
		if (moveStored == 0) {
			*lastRow = (size - 1);
			*lastCol = j;
			board[(size - 1)][j] = player;
		}
	}

	// Repeat the same steps for the East side
	else if (side == 'E') {
		i = move;
		for (j = (size - 1); j >= 0; j--) {
			if (board[i][j] != 0) {
				if((j + 1) < size){
					*lastRow = i;
					*lastCol = (j + 1);
					board[i][(j + 1)] = player;
				}
				else {
					*lastRow = -1;
					*lastCol = -1;
				}
				moveStored = 1;
				break;
			}
		}
		if (moveStored == 0) {
			*lastRow = i;
			*lastCol = 0;
			board[i][0] = player;
		}
	}

	// Repeat the same steps for the South side
	else if (side == 'S') {
		j = move;
		for (i = (size - 1); i >= 0; i--) {
			if (board[i][j] != 0) {
				if((i + 1) < size){
					*lastRow = (i + 1);
					*lastCol = j;
					board[(i + 1)][j] = player;
				}
				else {
					*lastRow = -1;
					*lastCol = -1;
				}
				moveStored = 1;	
				break;
			}
		}
		if (moveStored == 0) {
			*lastRow = 0;
			*lastCol = j;
			board[0][j] = player;
		}
	}
	// Repeat the same steps for the West side
	else if (side == 'W') {
		i = move;
		for (j = 0; j < size; j++) {
			if (board[i][j] != 0) {
				if((j - 1) >= 0){
					*lastRow = i;
					*lastCol = (j - 1);
					board[i][(j - 1)] = player;
				}
				else {
					*lastRow = -1;
					*lastCol = -1;
				}
				moveStored = 1;
				break;
			}
		}
		if (moveStored == 0) {
			*lastRow = i;
			*lastCol = (size - 1);
			board[i][(size - 1)] = player;
		}
	}
}

int CheckGameOver(int board[MAX_SIZE][MAX_SIZE], int size, int player, int row, int col)
{
	int checkSpaces = 0;
	int values[7] = { 0 };
	int index = 0;
	int result;

	// Check if there are any spaces left available on the board, if not then the last player to make a move wins
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				checkSpaces = 1;
			}
		}
	}

	if (checkSpaces == 0) {
		return player;
	}
	
	// Check if there are four counters in a row in a vertical direction, if so then return last player to make a move
	for (int i = -3; i < 4; i++) { // Check three counters above and three counters below
		if (((row + i) >= 0) && ((row + i) < size)){
			values[index] = board[(row + i)][col];
			index++;
		}
	}

	result = FourInARow(values, index); // Call the FourInARow() function to check if there are four consecutive counters in this direction
	index = 0;

	if (result != -1) {
		if (values[result] == 1 || values[result] == 2) {
			return player;
		}
	}

	// Repeat steps above for the horizontal direction
	for (int i = -3; i < 4; i++) {
		if (((col + i) >= 0) && ((col + i) < size)) {
			values[index] = board[row][(col + i)];
			index++;
		}
	}

	result = FourInARow(values, index);
	index = 0;

	if (result != -1) {
		if (values[result] == 1 || values[result] == 2) {
			return player;
		}
	}

	// Repeat steps above for the diagonal directions
	for (int i = -3; i < 4; i++) {
		if (((col + i) >= 0) && ((col + i) < size) && ((row + i) >= 0) && ((row + i) < size)) {
			values[index] = board[(row + i)][(col + i)];
			index++;
		}
	}

	result = FourInARow(values, index);
	index = 0;

	if (result != -1) {
		if (values[result] == 1 || values[result] == 2) {
			return player;
		}
	}

	for (int i = -3; i < 4; i++) {
		if (((col - i) >= 0) && ((col - i) < size) && ((row + i) >= 0) && ((row + i) < size)) {
			values[index] = board[(row + i)][(col - i)];
			index++;
		}
	}

	result = FourInARow(values, index);
	index = 0;

	if (result != -1) {
		if (values[result] == 1 || values[result] == 2) {
			return player;
		}
	}

	return 0;
}

void GetDisplayBoardString(int board[MAX_SIZE][MAX_SIZE], int size, char *boardString)
{
	int index = 0;
	int countRow = 0;
	int i;
	int j;

	char numChar[2];

	// Append characters that make up board to a string to be displayed in the output window
	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;

	// Append North indicator
	for (i = 0; i < size; i++) {
		boardString[index] = 'N';
		index++;
	}

	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;
	boardString[index] = '\n';
	index++;
	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;

	// Append top column numbers
	for (i = 0; i < size; i++) {
		sprintf(numChar, "%i", i);
		boardString[index] = numChar[0];
		index++;
	}

	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;
	boardString[index] = '\n';
	index++;

	// Append East and West indicators, row numbers and the apporiate character depending on the value stored at the current 
	// location in the board array
	for (i = 0; i < size; i++) {
		boardString[index] = 'W';
		index++;
		sprintf(numChar, "%i", countRow);
		boardString[index] = numChar[0];
		index++;
		countRow++;

		for (j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				boardString[index] = '.';
			}
			else if (board[i][j] == 1) {
				boardString[index] = 'X';
			}
			else if (board[i][j] == 2) {
				boardString[index] = 'O';
			}
			else if (board[i][j] == 3) {
				boardString[index] = '#';
			}
			index++;
		}

		sprintf(numChar, "%i", (countRow - 1));
		boardString[index] = numChar[0];
		index++;
		boardString[index] = 'E';
		index++;
		boardString[index] = '\n';
		index++;
	}

	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;

	// Append bottom row numbers and South direction indicator
	for (i = 0; i < size; i++) {
		sprintf(numChar, "%i", i);
		boardString[index] = numChar[0];
		index++;
	}

	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;
	boardString[index] = '\n';
	index++;
	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;

	for (i = 0; i < size; i++) {
		boardString[index] = 'S';
		index++;
	}

	boardString[index] = '-';
	index++;
	boardString[index] = '-';
	index++;
	boardString[index] = '\n';
	index++;
	boardString[index] = '\0';
}

int CountCounters(int values[7], int length, int player) {
	int count = 0;
	int temp = 0;

	// Check, from each index position, if there are four consecutive elements with the same value in the values array
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < 4; j++) {
			if ((values[(i + j)] == player) && ((i + j) < 7)) {
				temp++;
			}
			else {
				break;
			}
		}
		if (temp > count) {
			count = temp;
		}
		temp = 0;
	}
	return count;
}

int CountPattern(int board[MAX_SIZE][MAX_SIZE], int size, int row, int col, int player) {
	int values[7] = {0};
	int index = 0;
	int result = 0;
	int tempResults = 0;
	
	// Add values in each direction before and after a location on the board to see if there are four consecutive elements 
	// with the same value
	
	// Check in the vertical direction
	for (int i = -3; i < 4; i++) {
		if (((row + i) >= 0) && ((row + i) < size)) {
			values[index] = board[(row + i)][col];
			index++;
		}
	}

	tempResults = CountCounters(values, index, player); // Check to see if there are four consecutive elements in the array with the same value

	// Store the highest number of consecutive values in all directions
	if (tempResults > result) {
		result = tempResults;
	}

	index = 0;

	// Check in the horizontal direction
	for (int i = -3; i < 4; i++) {
		if (((col + i) >= 0) && ((col + i) < size)) {
			values[index] = board[row][(col + i)];
			index++;
		}
	}

	tempResults = CountCounters(values, index, player);

	if (tempResults > result) {
		result = tempResults;
	}

	index = 0;

	// Check in diagonal directions
	for (int i = -3; i < 4; i++) {
		if (((col + i) >= 0) && ((col + i) < size) && ((row + i) >= 0) && ((row + i) < size)) {
			values[index] = board[(row + i)][(col + i)];
			index++;
		}
	}

	tempResults = CountCounters(values, index, player);

	if (tempResults > result) {
		result = tempResults;
	}

	index = 0;

	for (int i = -3; i < 4; i++) {
		if (((col - i) >= 0) && ((col - i) < size) && ((row + i) >= 0) && ((row + i) < size)) {
			values[index] = board[(row + i)][(col - i)];
			index++;
		}
	}

	tempResults = CountCounters(values, index, player);

	if (tempResults > result) {
		result = tempResults;
	}

	return result;
}

char MakeMove(int board[MAX_SIZE][MAX_SIZE], int size, int row, int col) {
	int tryColumn = 0;
	int tryRow = 0;

	// Check to see if position on board can be reached from the North side
	tryRow = -1;
	tryColumn = col;
	while ((board[(tryRow + 1)][tryColumn] == 0) && ((tryRow + 1) < size)) {
		tryRow++;
	}

	if (tryRow == row) { // If the specified row can be reached then the bot move can be made from the North side
		return 'N';
	}

	// Check to see if position on boar can be reached from the East side
	tryColumn = size;
	tryRow = row;
	while ((board[tryRow][(tryColumn - 1)] == 0) && ((tryColumn - 1) >= 0)) {
		tryColumn--;
	}

	if (tryColumn == col) { // If the specified row can be reached then the bot move can be made from the East side
		return 'E';
	}

	// Check to see if position on board can be reached from the South side
	tryColumn = col;
	tryRow = size;
	while ((board[(tryRow - 1)][tryColumn] == 0) && ((tryRow - 1) >= 0)) {
		tryRow--;
	}

	if (tryRow == row) { // If the specified row can be reached then the bot move can be made from the South side
		return 'S';
	}

	// Check to see if position on boar can be reached from the West side
	tryColumn = -1;
	tryRow = row;
	while ((board[tryRow][(tryColumn + 1)] == 0) && ((tryColumn + 1) < size)) {
		tryColumn++;
	}

	if (tryColumn == col) { // If the specified row can be reached then the bot move can be made from the West side
		return 'W';
	}

	return 'X';
}

void saveMove(char successfulMove, int testCol, int testRow, char* side, int* move) {
	// Save move made by bot using side and move pointers
	if (successfulMove == 'N' || successfulMove == 'S') {
		*side = successfulMove;
		*move = testCol;
	}
	else if (successfulMove == 'E' || successfulMove == 'W') {
		*side = successfulMove;
		*move = testRow;
	}
}

int FindPlayerCounters(int board[MAX_SIZE][MAX_SIZE], int size, int player, int count, char* side, int* move) {
	char successfulMove;
	int testRow;
	int testCol;
	int countResult;

	// Find each counter placed by player and check if it forms a pattern with a length, specified by the count parameter, 
	// consecutive counters placed by player

	for (int row = 0; row < size; row++) {
		for (int col = 0; col < size; col++) {

			testRow = row;
			testCol = col;

			if (board[row][col] == player) {

				// Check counters above and below current player counter
				if ((board[(row + 1)][col] == 0) && ((row + 1) < size)) {
					testRow = row + 1;


					board[testRow][col] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[testRow][col] = 0;
					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}

				}

				if ((board[(row - 1)][col] == 0) && ((row - 1) >= 0)) {
					testRow = row - 1;

					board[testRow][col] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[testRow][col] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}
				}
				testRow = row; 

				// Check counters left and right of current player counter
				if ((board[row][(col + 1)] == 0) && ((col + 1) < size)) {
					testCol = col + 1;

					board[row][testCol] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[row][testCol] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}

				}

				if ((board[row][(col - 1)] == 0) && ((col - 1) >= 0)) {
					testCol = col - 1;

					board[row][testCol] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[row][testCol] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}

				}

				// Check counters in diagonal directions from current player counter
				if ((board[(row + 1)][(col + 1)] == 0) && ((row + 1) < size) && ((col + 1) < size)) {
					testRow = row + 1;
					testCol = col + 1;

					board[testRow][testCol] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[testRow][testCol] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}

				}
				if ((board[(row - 1)][(col - 1)] == 0) && ((row - 1) >= 0) && ((col - 1) >= 0)) {
					testRow = row - 1;
					testCol = col - 1;

					board[testRow][testCol] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[testRow][testCol] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}

				}

				if ((board[(row + 1)][(col - 1)] == 0) && ((row + 1) < size) && ((col - 1) >= 0)) {
					testRow = row + 1;
					testCol = col - 1;

					board[testRow][testCol] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[testRow][testCol] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}

				}
				if ((board[(row - 1)][(col + 1)] == 0) && ((row - 1) >= 0) && ((col + 1) < size)) {
					testRow = row - 1;
					testCol = col + 1;

					board[testRow][testCol] = player;
					countResult = CountPattern(board, size, row, col, player);

					board[testRow][testCol] = 0;

					if (countResult == count) {
						successfulMove = MakeMove(board, size, testRow, testCol);
						if (successfulMove != 'X') {
							saveMove(successfulMove, testCol, testRow, side, move);
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

void GetMoveBot1(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move) {	
	int stopCount;
	char successfulMove;
	int functionReturn;

	// Check if a winning move can be made by bot, if so try make the move to that position
	functionReturn = FindPlayerCounters(board, size, player, 4, side, move);

	if (functionReturn == 1) {
		return;
	}
	
	if (player == 1) {
		player = 2;
	}
	else {
		player = 1;
	}

	// If a winning move can not be made by bot then block other player from making moves that would result in then 
	// getting a pattern of 3 or 4 consecutive counters
	for (stopCount = 4; stopCount > 2; stopCount--) {
		functionReturn = FindPlayerCounters(board, size, player, stopCount, side, move);
		if (functionReturn == 1) {
			return;
		}
	}

	if (player == 2) {
		player = 1;
	}
	else {
		player = 2;
	}
	
	// If a winning move can't be made and the other player doesn't need to be blocked then make a move to a position 
	// which will result on the longest consecutive pattern of counters 
	for(int MaxCount = 3; MaxCount > 0; MaxCount--){
		functionReturn = FindPlayerCounters(board, size, player, MaxCount, side, move);
		if (functionReturn == 1) {
			return;
		}
	}

	// If none of the conditions above can be met then make a move to any empty position on the board
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				if (board[(i + 1)][j] == 0) {
					i++;
				}
				else if (board[(i - 1)][j] == 0) {
					i--;
				}
				else if (board[i][(j + 1)] == 0) {
					j++;
				}
				else if (board[i][(j - 1)] == 0) {
					j--;
				}
				else if (board[(i + 1)][(j + 1)] == 0) {
					i++;
					j++;
				}
				else if (board[(i - 1)][(j - 1)] == 0) {
					i--;
					j--;
				}
				else if (board[(i + 1)][(j - 1)] == 0) {
					i++;
					j--;
				}
				else if (board[(i - 1)][(j + 1)] == 0) {
					i--;
					j++;
				}

				successfulMove = MakeMove(board, size, i, j);

				if (successfulMove != 'X') {
					if (successfulMove == 'N' || successfulMove == 'S') {
						*side = successfulMove;
						*move = j;
					}
					else if (successfulMove == 'E' || successfulMove == 'W') {
						*side = successfulMove;
						*move = i;
					}
					return;
				}
				return;
			}
		}
	}
}

void GetMoveBot2(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move)
{
	int stopCount;
	char successfulMove;
	int functionReturn;

	// Check if a winning move can be made by bot, if so try make the move to that position
	functionReturn = FindPlayerCounters(board, size, player, 4, side, move);

	if (functionReturn == 1) {
		return;
	}

	if (player == 1) {
		player = 2;
	}
	else {
		player = 1;
	}

	// If a winning move can not be made by bot then block other player from making moves that would result in then 
	// getting a pattern of 3 or 4 consecutive counters
	for (stopCount = 4; stopCount > 2; stopCount--) {
		functionReturn = FindPlayerCounters(board, size, player, stopCount, side, move);
		if (functionReturn == 1) {
			return;
		}
	}

	if (player == 2) {
		player = 1;
	}
	else {
		player = 2;
	}

	// If a winning move can't be made and the other player doesn't need to be blocked then make a move to a position 
	// which will result on the longest consecutive pattern of counters 
	for (int MaxCount = 3; MaxCount > 0; MaxCount--) {
		functionReturn = FindPlayerCounters(board, size, player, MaxCount, side, move);
		if (functionReturn == 1) {
			return;
		}
	}

	// If none of the conditions above can be met then make a move to any empty position on the board
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (board[i][j] == 0) {
				if (board[(i + 1)][j] == 0) {
					i++;
				}
				else if (board[(i - 1)][j] == 0) {
					i--;
				}
				else if (board[i][(j + 1)] == 0) {
					j++;
				}
				else if (board[i][(j - 1)] == 0) {
					j--;
				}
				else if (board[(i + 1)][(j + 1)] == 0) {
					i++;
					j++;
				}
				else if (board[(i - 1)][(j - 1)] == 0) {
					i--;
					j--;
				}
				else if (board[(i + 1)][(j - 1)] == 0) {
					i++;
					j--;
				}
				else if (board[(i - 1)][(j + 1)] == 0) {
					i--;
					j++;
				}

				successfulMove = MakeMove(board, size, i, j);

				if (successfulMove != 'X') {
					if (successfulMove == 'N' || successfulMove == 'S') {
						*side = successfulMove;
						*move = j;
					}
					else if (successfulMove == 'E' || successfulMove == 'W') {
						*side = successfulMove;
						*move = i;
					}
					return;
				}
				return;
			}
		}
	}
}
