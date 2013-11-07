#include <msp430g2553.h>
#include "game.h"
#include "LCD_Driver.h"

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

