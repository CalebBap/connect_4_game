// Author: Caleb Baptista

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 10

/*********************************************************************************/
// Function prototype declarations
int SecondPlacePrize(int prize1, int prize2, int prize3);
int FourInARow(int values[], int length);
int BinaryToDecimal(int binary);
double MedianAbility(double abilities[], int length);
void RemoveSpaces(char *name);
void InitialiseBoard(int board[MAX_SIZE][MAX_SIZE], int size);
void AddMoveToBoard(int board[MAX_SIZE][MAX_SIZE], int size, char side, int move, int player, int *lastRow, int *lastCol);
int CheckGameOver(int board[MAX_SIZE][MAX_SIZE], int size, int player, int row, int col);
void GetDisplayBoardString(int board[MAX_SIZE][MAX_SIZE], int size, char *boardString);
void GetMoveBot1(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move);
void GetMoveBot2(int board[MAX_SIZE][MAX_SIZE], int size, int player, char *side, int *move);
/*********************************************************************************/

