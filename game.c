#include <msp430g2553.h>
#include "game.h"
#include "LCD_Driver.h"
#include "rand.h"
/*
 * game.h
 * Author: John Miller
 *
 * Functions for the game titled "simple game 5: A lab", player is on an 8x2 screen and must navigate from the top left to the bottom right.
 * But beware, there are mines in place, as well as a 2 second time limit in-between moves!
 */
unsigned int random = 4352;

unsigned char initPlayer() {
	return 0x80;
}

void printPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte('*');
}

void clearPlayer(unsigned char player) {
	writeCommandByte(player);
	writeDataByte(' ');
}

//The difference between the addresses on the top and bottom row is 0x40, so I found it easier to add or subtract that to change rows
unsigned char movePlayer(unsigned char player, unsigned char direction) {
	switch (direction) {
	case UP:
		if (player > 0x87) {
			clearPlayer(player);
			player -= 0x40;

			printPlayer(player);
			debounce();

		}
		break;
	case DOWN:
		if (player < 0xC0) {
			clearPlayer(player);
			player += 0x40;

			printPlayer(player);
			debounce();

		}
		break;
	case LEFT:
		if ((player > 0xC0 && player <= 0xC7)
				|| (player > 0x80 && player <= 0x87)) {
			clearPlayer(player);
			player -= 0x01;

			printPlayer(player);
			debounce();

		}
		break;
	case RIGHT:
		if ((player >= 0xC0 && player < 0xC7)
				|| (player >= 0x80 && player < 0x87)) {
			clearPlayer(player);
			player += 0x01;

			printPlayer(player);
			debounce();
		}
		break;

	}

	return player;
}

char didPlayerWin(unsigned char player) {
	return player == 0xC7;
}

char didPlayerHitMine(unsigned char player, unsigned char mines[NUM_MINES]) {
	int i;
	for (i = 0; i < NUM_MINES; i++) {
		if (player == mines[i]) {
			return 1;
		}

	}
	return 0;
}

//Generates mines on the top and bottom rows in valid locations
void generateMines(unsigned char * mines) {
	random = prand(random);
	char mine_1_location = 0x81 + random % 7; //0x80 is the starting location, so everything to the right of it is valid
	random = prand(random);
	char mine_2_location = 0xc0 + random % 7; //0xc7 is the ending location, so everything to the left of it is valid
	while (invalidMineCheck(mine_1_location, mine_2_location)) {
		random = prand(random);
		mine_2_location = 0xc0 + random % 6;
	}
	mines[0] = mine_1_location;
	mines[1] = mine_2_location;
	printMines(mines);

}

void printMines(unsigned char * mines) {
	int i;
	for (i = 0; i < NUM_MINES; i++) {
		writeCommandByte(mines[i]);
		writeDataByte('X');

	}
}

char invalidMineCheck(char mine1, char mine2) {
	return ((mine1 + 0x40 == mine2) || (mine1 + 0x39 == mine2)
			|| (mine1 + 0x41 == mine2) || mine2 == 0xc7);
}
