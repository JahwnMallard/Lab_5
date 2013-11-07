#include <msp430.h> 
#include "LCD_Driver.h"
#include "game.h"
/*
 * main.c
 */

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	char buttons[] = { BIT0, BIT1, BIT2, BIT3 };

	initProgram();
	initPlayer();
	printPlayer();

	while (timer < 4 || !didPlayerWin()) {
		int i;
		for (i = 0; i < 4; i++) {
			char button = isP1ButtonPressed(buttons[i]);
			debounce();

		}
		switch (button) {
		case BIT0:
			movePlayer(player, RIGHT);
			timer = 0;
			break;
		case BIT1:
			movePlayer(player, LEFT);
			timer = 0;
			break;
		case BIT2:
			movePlayer(player, UP);
			timer = 0;
			break;
		case BIT3:
			movePlayer(player, DOWN);
			timer = 0;
			break;
		}

	}

	if(timer=4){
		endGameLose();
	}else if(didPlayerWin()){

	}

	// Flag for continuous counting is TAIFG
	#pragma vector=TIMER0_A1_VECTOR
	__interrupt void TIMER0_A1_ISR()
	{
	    TACTL &= ~TAIFG;            // clear interrupt flag
	    timer++;
	}
	return 0;
}
