#include <msp430g2553.h>
#include "game.h"
#include "LCD_Driver.h"
#include "rand.h"

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

unsigned char movePlayer(unsigned char player, unsigned char direction) {
	switch (direction) {
	case UP:
		if (player > 0x80) {
			clearPlayer(player);
			player -= 0x40;

			printPlayer(player);

		}
		break;
	case DOWN:
		if (player < 0xC0) {
			clearPlayer(player);
			player += 0x40;

			printPlayer(player);

		}
		break;
	case LEFT:
		if ((player > 0xC0 && player <= 0xC7)
				|| (player > 0x80 && player <= 0x87)) {
			clearPlayer(player);
			player -= 0x01;

			printPlayer(player);

		}
		break;
	case RIGHT:
		if ((player >= 0xC0 && player < 0xC7)
				|| (player >= 0x80 && player < 0x87)) {
			clearPlayer(player);
			player += 0x01;

			printPlayer(player);

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

void generateMines(unsigned char mines[NUM_MINES]) {
	int state = prand(INITIAL_SEED);
	int mine_1_location = 0x81 + state % 6;
	state = prand(state);
	int mine_2_location = 0xc0 + state % 6;
	while (invalidMineCheck(mine_1_location, mine_2_location)) {
		state = prand(state);
		mine_2_location = 0xc0 + state % 6;
	}
	mines[0] = mine_1_location;
	mines[1] = mine_2_location;
	printMines(mines);
}

void printMines(unsigned char mines[NUM_MINES]) {
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
