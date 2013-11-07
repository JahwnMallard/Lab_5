#include <msp430.h> 
#include "LCD_Driver.h"
#include "game.h"
/*
 * main.c
 */
int timer = 0;
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	char * endMessageTop = "You am";
	char * endMessageBottomLose = "lose";
	char * endMessageBottomWin = "Not lose";
	char buttons[] = { BIT0, BIT1, BIT2, BIT3 };

	initProgram();
	unsigned char player = initPlayer();
	printPlayer(player);

	while (timer < 4 && !didPlayerWin(player)) {

		char button;

		button = pollP1Buttons( buttons,  4);

		switch (button) {
		case BIT0:
			player =movePlayer(player, RIGHT);
			timer = 0;
			break;
		case BIT1:
			player = movePlayer(player, LEFT);
			timer = 0;
			break;
		case BIT2:
			player = movePlayer(player, UP);
			timer = 0;
			break;
		case BIT3:
			player = movePlayer(player, DOWN);
			timer = 0;
			break;
		}

	}

	if (timer == 4) {
		_disable_interrupt();
		cursorToLineOne();
		writeString(endMessageTop, 8);
		cursorToLineTwo();
		writeString(endMessageBottomLose, 8);
	} else if (didPlayerWin(player)) {
		_disable_interrupt();
		cursorToLineOne();
		writeString(endMessageTop, 8);
		cursorToLineTwo();
		writeString(endMessageBottomWin, 8);
	}

	// Flag for continuous counting is TAIFG

	return 0;
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
	timer++;
}
