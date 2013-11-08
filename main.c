#include <msp430.h> 
#include "LCD_Driver.h"
#include "game.h"
/*
 * main.c
 */
int timer;
char button;
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	char * endMessageTop = "You am  ";
	char * endMessageBottomLose = "lose    ";
	char * endMessageBottomWin = "Not lose";
	char * deadMessage = "Dead    ";
	unsigned char player;
	unsigned char  mines[NUM_MINES];

	while (1)
	{
		timer = 0;
		button = 0;
		player = initPlayer();
		initProgram();
		printPlayer(player);
		generateMines(mines);

	while ((timer < 4) && !didPlayerWin(player)
			&& !didPlayerHitMine(player, mines))
	{

		//	button = pollP1Buttons( buttons,  4);
		if (button)
		{
			switch (button) {
				case BIT0:
					player = movePlayer(player, RIGHT);
					break;
				case BIT1:
					player = movePlayer(player, LEFT);
					break;
				case BIT2:
					player = movePlayer(player, UP);
					break;
				case BIT3:
					player = movePlayer(player, DOWN);
					break;
				}
			button = 0;
		}
	}

	//if the timer hits 2 seconds, or if the player hits a mine, the game is over
		if (timer == 4 || didPlayerHitMine(player, mines)) {

			//displays a special message if the player lost due to hitting a mine
			if(didPlayerHitMine(player, mines)){
				cursorToLineOne();
				writeString(deadMessage, 8);
				_delay_cycles(500000);
			}
			button = 0;

			//displayes game over message
			cursorToLineOne();
			writeString(endMessageTop, 8);
			cursorToLineTwo();
			writeString(endMessageBottomLose, 8);
		} else if (didPlayerWin(player)) {  //if the player hits 0xC7 they win, display the win message
			button = 0;
			cursorToLineOne();
			writeString(endMessageTop, 8);
			cursorToLineTwo();
			writeString(endMessageBottomWin, 8);
		}


	// Waits for a button input and then resets the game
	while (button != 0)
	{
	}
}
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_ISR() {
	TACTL &= ~TAIFG;            // clear interrupt flag
	timer++;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1_ISR(void) {
	if (BIT0 & P1IFG) {
		if (BIT0 & P1IES) {
			button = BIT0;
			timer = 0;
		} else {
			debounce();
		}

		P1IES ^= BIT0;
		P1IFG &= ~BIT0;
	}

	if (BIT1 & P1IFG) {
		if (BIT1 & P1IES) {
			button = BIT1;
			timer = 0;
		} else {
			debounce();
		}

		P1IES ^= BIT1;
		P1IFG &= ~BIT1;
	}

	if (BIT2 & P1IFG) {
		if (BIT2 & P1IES) {
			button = BIT2;
			timer = 0;
		} else {
			debounce();
		}

		P1IES ^= BIT2;
		P1IFG &= ~BIT2;
	}

	if (BIT3 & P1IFG) {
		if (BIT3 & P1IES) {
			button = BIT3;
			timer = 0;
		} else {
			debounce();
		}

		P1IES ^= BIT3;
		P1IFG &= ~BIT3;
	}
}
