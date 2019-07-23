/*	Author: xwang296
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

enum left_bar_status {init, wait, movingUp, movingDown} l_status;

void left_bar() {
	unsigned char left_button = ~PINA & 0x0C; // 0000 1100
	unsigned char tempC;
	unsigned char tempD;
	switch (l_status) {
		case init:
			l_status = wait;
		case wait:
			if(left_button == 0x08) {
				l_status = movingUp;
			} else if (left_button == 0x04) {
				l_status = movingDown;
			} else {
				l_status = wait;
			}
			break;
		case movingUp:
			if(left_button != 0x08) {
				l_status = wait;
			}
			break;
		case movingDown:
			if(left_button != 0x04) {
				l_status = wait;
			}
			break;
		default:
			l_status = wait;
			break;
	}
	
	switch (l_status) {
		case init:
			tempC = 0x8C;
			tempD = 0x45;
			break;
		case wait:
			break;
		case movingUp:
			break;
		case movingDown:
			break;
		default:
			break;
	}
	PORTC = tempC;
	PORTD = tempD;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

    /* Insert your solution below */
    TimerSet(5);
    TimerOn();
    while (1) {
		left_bar();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
