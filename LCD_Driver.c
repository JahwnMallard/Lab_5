/*
 * Author: C2C John Miller
 *   LCD Driver functions
 *
 *  Created on: Oct 22, 2013
 *      Author: Administrator
 */

#include <msp430.h>;
#include "LCD_Driver.h";
#include "button.h";
#define RS_MASK 0x40
#define LCD_WIDTH 8
char LCDCON;

void writeDataByte(char dataByte);

void writeCommandNibble(char commandNibble);

void writeCommandByte(char commandByte);

void initButtons() {
	configureP1PinAsButton(BIT0 | BIT1 | BIT2 | BIT3);
}

void initProgram() {
	initSPI();
	LCDinit();
	LCDclear();
	initButtons();
}

void initSPI()

{
	UCB0CTL1 |= UCSWRST;
	UCB0CTL0 |= UCCKPL | UCMSB | UCMST | UCSYNC;

	UCB0CTL1 |= UCSSEL1; //Selects which clock to use
	//UCB0STAT |= UCLISTEN; //enables internal loopback

	P1DIR |= BIT4; //P1.4 is used as the slave select

	P1SEL |= BIT6; //make UCB0SSOMI available on P1.6
	P1SEL2 |= BIT6;

	P1SEL |= BIT5; //make UCB0CLK available on P1.5
	P1SEL2 |= BIT5;

	P1SEL |= BIT7; //make UCB0SSIMO available on P1.7
	P1SEL2 |= BIT7;

	UCB0CTL1 &= ~UCSWRST; //enable subsystem

	TACTL &= ~(MC1 | MC0);        // stop timer

	TACTL |= TACLR;             // clear TAR

	TACTL |= TASSEL1;   // configure for SMCLK - what's the frequency (roughly)?

	TACTL |= ID1 | ID0; // divide clock by 8 - what's the frequency of interrupt?

	TACTL &= ~TAIFG;            // clear interrupt flag

	TACTL |= MC1;               // set count mode to continuous

	TACTL |= TAIE;              // enable interrupt

	__enable_interrupt();       // enable maskable interrupts
}

void LCDclear() {
	writeCommandByte(1);
}

void LCDinit() {
	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

	writeCommandNibble(0x03);

	writeCommandNibble(0x02);

	writeCommandByte(0x28);

	writeCommandByte(0x0C);

	writeCommandByte(0x01);

	writeCommandByte(0x06);

	writeCommandByte(0x01);

	writeCommandByte(0x02);

	SPI_send(0);
	delayMicro();
}

void writeCommandNibble(char commandNibble) {
	LCDCON &= ~RS_MASK;
	LCD_write_4(commandNibble);
	delayMilli();
}

void writeCommandByte(char commandByte) {
	LCDCON &= ~RS_MASK;
	LCD_write_8(commandByte);
	delayMilli();
}

void writeDataByte(char dataByte) {
	LCDCON |= RS_MASK;
	LCD_write_8(dataByte);
	delayMilli();
}

void writeChar(char asciiChar) {
	writeCommandByte(0x06); //cursor increment
	writeDataByte(asciiChar);
}

void writeString(char * string, int length) {
	int i;
	for (i = 0; i < length; i++) {
		writeChar(string[i]);
	}

}

void calibrateClock(char frequency) {
	DCOCTL = 0; // Select lowest DCOxand MODx settings
	switch (frequency) {

	case 1:
		BCSCTL1 = CALBC1_1MHZ;
		DCOCTL = CALDCO_1MHZ;
		break;

	case 8:
		BCSCTL1 = CALBC1_8MHZ;
		DCOCTL = CALDCO_8MHZ;
		break;

	case 12:
		BCSCTL1 = CALBC1_12MHZ;
		DCOCTL = CALDCO_12MHZ;
		break;

	case 16:
		BCSCTL1 = CALBC1_16MHZ;
		DCOCTL = CALDCO_16MHZ;
		break;

	}

}

void scrollString(char * string1, char * string2, int message1Length,
		int message2Length) {
	int i = 0;

	while (1) {

		if (i % message1Length + LCD_WIDTH <= message1Length) {
			writeString((string1 + i % message1Length), LCD_WIDTH);

		} else {
			writeString(string1 + i % message1Length,
					message1Length - i % message1Length);
			writeString(string1, i % message1Length);
		}
		cursorToLineTwo();

		if (i % message2Length + LCD_WIDTH <= message2Length) {
			writeString((string2 + i % message2Length), LCD_WIDTH);

		} else {
			writeString(string2 + i % message2Length,
					message2Length - i % message2Length);
			writeString(string2, i % message2Length);
		}
		cursorToLineOne();
		//resets the cursor
		i++;
	}

}
void LCD_write_8(char byteToSend) {
	unsigned char sendByte = byteToSend;

	sendByte &= 0xF0;

	sendByte = sendByte >> 4;               // rotate to the right 4 times

	LCD_write_4(sendByte);

	sendByte = byteToSend;

	sendByte &= 0x0F;

	LCD_write_4(sendByte);
}

void LCD_write_4(unsigned char sendByte) {
	sendByte &= 0x0F;

	sendByte |= LCDCON;

	sendByte &= 0x7F;

	SPI_send(sendByte);

	delayMicro();

	sendByte |= 0x80;

	SPI_send(sendByte);

	delayMicro();

	sendByte &= 0x7F;

	SPI_send(sendByte);

	delayMicro();

}

void set_SS_hi() {
	P1OUT |= BIT4;  //Disables slave select
}

void set_SS_lo() {
	P1OUT &= ~BIT4;  //Enables slave select
}

void SPI_send(char byteToSend) {
	volatile char readByte;

	set_SS_lo();

	UCB0TXBUF = byteToSend;

	while (!(UCB0RXIFG & IFG2)) {
		// wait until you've received a byte
	}

	readByte = UCB0RXBUF;

	set_SS_hi();
}

void delayMicro() {
	__delay_cycles(43);
}

void delayMilli() {
	__delay_cycles(1743);
}

void cursorToLineOne() {
	writeCommandByte(0x02);
}

void cursorToLineTwo() {
	cursorToLineOne();
	int i;
	for (i = 0; i < 40; i++) {
		writeCommandByte(0x14);
	}
}

