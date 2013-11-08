#Lab5
##API

-void `initPlayer()`
	-Starts off the player at the top left position on the LCD (address 0x80)

-void `printPlayer(unsigned char player)`
	-Displays the player at the position passed in. 
	-Positions 0x80 - 0x87 are the top row, 0xC0 - 0xC7 are the bottom row.
	
-void `clearPlayer(unsigned char player)`
	-Deletes the player at the position passed in

-unsigned char `movePlayer(unsigned char player, unsigned char direction)`
	-Takes in the players current position, moves them from there to the new position based off of the direction passed in
	-Erases the player at the old position
	
-char `didPlayerWin(unsigned char player)`
	-Checks if the player is at 0xC7, the bottom right of the LCD screen.
	-If so, the player has won, and it triggers the win sequence

-char `didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES])`
	-Compares the position of the player to the position of the mines. If they are the same, the player has lost.

-void `generateMines(unsigned char * mines)`
	-Randomly generates a mine on the top and bottom row.
	-The mines cannot be generated where the player is, nor can they be stacked or diagonal to each other. 

-char `invalidMineCheck(char mine1, char mine2)`
	-Checks if the mine has been placed in an invalid location.

-void `printMines(unsigned char mines[NUM_MINES])`
	-Places the mines on the LCD.
