/*
 * game.h
 * Author: Todd Branchflower
 *
 * This is shell code for the game to be implemented in Lab 4.  It provides basic functions that may prove useful in accomplishing the lab.
 */

#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

#define ROW_MASK 0x40

#define NUM_MINES 2

#define INITIAL_SEED 2874

//
// Initializes player to starting position on board.
//
unsigned char initPlayer();

//
// Prints the player marker (*) at the player location passed in.
//
void printPlayer(unsigned char player);

//
// Clears the location passed in.
//
void clearPlayer(unsigned char player);

//
// Given a player's current position and a direction, returns an updated player position.
//
unsigned char movePlayer(unsigned char player, unsigned char direction);

//
// Returns true if the position passed in is the winning game position.
//
char didPlayerWin(unsigned char player);

//checks if the player has hit a mine
char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]);

//creates 2 mines at random locations, one on the top row, one on the bottom.
//The mines cannot be stacked or diagonal to each other.
//The mines cannot be placed in the starting or ending locations
void generateMines(unsigned char * mines);

//checks whether or not the bottom mine is placed in a valid location in relation to the first mine
char invalidMineCheck(char mine1, char mine2);

//displays the mines on the screen
void printMines(unsigned char mines[NUM_MINES]);

