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
#include <util/delay.h>
#include "scheduler.h"
#include "led_matrix.h"

uint8_t led_bot_row = 0b00000001;
uint8_t led_bot_column = 0b11000111;

uint8_t led_up_row = 0b10000000;
uint8_t led_up_column = 0b11000111;

static task task1, task2, task3, task4;
task *tasks[] = {&task1, &task2, &task3, &task4};

enum button_state {button_wait, button_up_press, button_up_release, button_down_press, button_down_release};
int bot_button_move(int state) {
	unsigned char tempA = ~PINA;
	unsigned char bot_up = tempA & 0x01;
	unsigned char bot_down = tempA & 0x02;  
	
	switch(state) {
		case button_wait:
			if((bot_up == 0x01) && (bot_down != 0x02))
				state = button_up_press;
			else if((bot_up != 0x01) && (bot_down == 0x02))
				state = button_down_press;
			else
				state = button_wait;
			break;
		case button_up_press:
			state = button_up_release;
			break;
		case button_up_release:
			if(bot_up == 0x01)
				state = button_up_release;
			else
				state = button_wait;
			break;
		case button_down_press:
			state = button_down_release;
			break;
		case button_down_release:
			if(bot_down == 0x02)
				state = button_down_release;
			else
				state = button_wait;
			break;
		default:
			state = button_wait;
			break;
	}
	switch(state) {
		case button_wait:
			break;
		case button_up_press:
			if((led_bot_column & 0x01) == 0x01) {
				led_bot_column = led_bot_column >> 1;
				led_bot_column = led_bot_column | 0x80;
			}
			break;
		case button_up_release:
			break;
		case button_down_press:
			if((led_bot_column & 0x80) == 0x80) {
				led_bot_column = led_bot_column << 1;
				led_bot_column = led_bot_column | 0x01;
			}
			break;
		case button_down_release:
			break;
		default:
			break;
	}
	return state;
}

int up_button_move(int state) {
	unsigned char tempA = ~PINA;
	unsigned char up_up = tempA & 0x04;
	unsigned char up_down = tempA & 0x08;
	
	switch(state) {
		case button_wait:
			if((up_up == 0x04) && (up_down != 0x08))
			state = button_up_press;
			else if((up_up != 0x04) && (up_down == 0x08))
			state = button_down_press;
			else
			state = button_wait;
		break;
			case button_up_press:
			state = button_up_release;
		break;
			case button_up_release:
			if(up_up == 0x04)
			state = button_up_release;
			else
			state = button_wait;
			break;
		case button_down_press:
			state = button_down_release;
			break;
			case button_down_release:
			if(up_down == 0x08)
			state = button_down_release;
			else
			state = button_wait;
			break;
		default:
			state = button_wait;
			break;
	}
	switch(state) {
		case button_wait:
		break;
		case button_up_press:
		if((led_up_column & 0x80) == 0x80) {
			led_up_column = led_up_column << 1;
			led_up_column = led_up_column | 0x01;
		}
		break;
		case button_up_release:
		break;
		case button_down_press:
		if((led_up_column & 0x01) == 0x01) {
			led_up_column = led_up_column >> 1;
			led_up_column = led_up_column | 0x80;
		}
		break;
		case button_down_release:
		break;
		default:
		break;
	}
	return state;
}

uint8_t led_pattern[10]={
	    0b00100000,
	    0b00010000,
	    0b00001000,
	    0b00000100,
	    0b00000010,
	    0b00000100,
	    0b00001000,
	    0b00010000,
	    0b00100000,
	    0b01000000,
    };
uint8_t led_pattern2[14]={
	    0b11111110,
	    0b11111101,
	    0b11111011,
	    0b11110111,
	    0b11101111,
	    0b11011111,
	    0b10111111,
	    0b01111111,
	    0b10111111,
	    0b11011111,
	    0b11101111,
	    0b11110111,
	    0b11111011,
	    0b11111101,
    };

uint8_t win_row[8]={
	0b10000000,
	0b01000000,
	0b00100000,
	0b00010000,
	0b00001000,
	0b00000100,
	0b00000010,
	0b00000001,
};
uint8_t up_win_column[8]={
	0b11111111,
	0b10011001,
	0b10011001,
	0b11111111,
	0b11111111,
	0b10111101,
	0b11000011,
	0b11111111,
};
uint8_t bot_win_column[8]={
	0b11111111,
	0b11000011,
	0b10111101,
	0b11111111,
	0b11111111,
	0b10011001,
	0b10011001,
	0b11111111,
};

unsigned char matrix_i = 0;
unsigned char matrix_j = 0;

enum display_state {display_wait, display, display_end};
const unsigned char ball_period = 10;
unsigned char ball_count = 0;
int display_ballSM(int state)
{
	switch(state)
	{
		case display_wait:
			if(ball_count == ball_period)
				state = display;
			break;
		case display:
			state = display_wait;
			break;
		default:
			break;
	}
	switch(state)
	{
		case display_wait:
			HC595Write2(led_pattern[matrix_i], led_pattern2[matrix_j]);
			ball_count++;
			break;
		case display:
			if(matrix_i == 9) matrix_i = 0;
			if(matrix_j == 13) matrix_j = 0;
			if(ball_count ==ball_period){
				matrix_i ++; matrix_j ++;
				ball_count = 0;
			}
			HC595Write2(led_pattern[matrix_i], led_pattern2[matrix_j]);//Write the data to HC595
			ball_count++;
			break;
		default:
			break; 
	}
	return state;
}

unsigned char up_win_counter = 0;
unsigned char bot_win_counter = 0;

const unsigned char reset_period = 200;

enum logic_state {game_wait, game_check, bot_win, up_win};
int game_logic(int state) {
	unsigned char count;
	switch(state) {
		case game_wait:
			state = game_check;
			break;
		case game_check:
				
			if(led_pattern[matrix_i] == 0x02) {
				if((led_pattern2[matrix_j] | led_bot_column) == 0xFF) {
					state = bot_win;
					return state;
				}
			}else if(led_pattern[matrix_i] == 0x40) {
				if((led_pattern2[matrix_j] | led_up_column) == 0xFF) {
					state = up_win;
					return state;
				}
			} else
				state = game_check;
			break;
		case bot_win:
			up_win_column[0] = up_win_column[0] << 1;
			up_win_column[0] = up_win_column[0] & 0xFE;
			bot_win_column[0] = bot_win_column[0] << 1;
			bot_win_column[0] = bot_win_column[0] & 0xFE;
			while(1) {
				for(uint8_t i=0;i<8;i++)
				HC595Write2(win_row[i], bot_win_column[i]);
				if(count > reset_period) {
					if((~PINA & 0x0F) != 0x00)
						break;
				}else{
					count++;	
				}
			}
			state = game_wait;
			break;
		case up_win:
			up_win_column[7] = up_win_column[7] << 1;
			up_win_column[7] = up_win_column[7] & 0xFE;
			bot_win_column[7] = bot_win_column[7] << 1;
			bot_win_column[7] = bot_win_column[7] & 0xFE;
			while(1) {
				for(uint8_t i=0;i<8;i++)
				HC595Write2(win_row[i], up_win_column[i]);
				if(count > reset_period) {
					if((~PINA & 0x0F) != 0x00)
					break;
					}else{
					count++;
				}
			}
			state = game_wait;
			break;
		default:
			state = game_wait;
			break;
	}
	
	switch(state) {
		case bot_win:
			break;
		case up_win:
			break;
		default:
			break;
	}
	return state;
}

int main()
{	
	DDRA = 0x00; PORTA = 0xFF;
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state =  button_wait;//Task initial state
	task1.period = 10;//Task Period
	task1.elapsedTime = task1.period;//Task current elapsed time.
	task1.TickFct = &bot_button_move;//Function pointer for the tick
	
	task2.state = display_wait;//Task initial state
	task2.period = 150;
	task2.elapsedTime = task2.period;//Task current elapsed time.
	task2.TickFct = &display_ballSM;//Function pointer for the tick
	
	task3.state = button_wait;//Task initial state
	task3.period = 10;//Task Period
	task3.elapsedTime = task3.period;//Task current elapsed time.
	task3.TickFct = &up_button_move;//Function pointer for the tick
	
	task4.state = game_wait;//Task initial state
	task4.period = ball_period*task2.period;//Task Period
	task4.elapsedTime = task4.period;//Task current elapsed time.
	task4.TickFct = &game_logic;//Function pointer for the tick
	
   //Initialize HC595 system
    HC595Init();
	
	unsigned long int GCD = 10;
	TimerSet(10);
	TimerOn();
	while(1)
	{
	   for(int k = 0; k < numTasks; k++){//Scheduler code
		   if(tasks[k]->elapsedTime == tasks[k]->period){//Task is ready to tick
			   tasks[k]->state= tasks[k]->TickFct(tasks[k]->state);//set next state
			   tasks[k]->elapsedTime = 0;//Reset the elapsed time for next tick;
		   }
		   tasks[k]->elapsedTime += GCD;
	   }
	   HC595Write2(led_bot_row,led_bot_column);
	   HC595Write2(led_up_row,led_up_column);
   }
   return 1;
}

