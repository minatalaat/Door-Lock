/*
 * final_project2.c
 *
 *  Created on: Oct 7, 2020
 *      Author: mina talaat
 */
#include "timer.h"
#include "uart.h"
#include "i2c.h"
#include "external_eeprom.h"
#include <util/delay.h>
uint16 g_motor_ticks = 0;
uint16 g_buzzer_ticks = 0;
void timer_interrupt(void){
	g_motor_ticks++;
	g_buzzer_ticks++;
	//we reach 20 seconds count from zero again
	if(g_motor_ticks >= 612){
		g_motor_ticks = 0;
	}
	if(g_buzzer_ticks >= 1836){
		g_buzzer_ticks = 0;
	}
}
int main(){
	uint8 pass[4];
	uint8 confirm_pass[4];
	uint8 i = 0;
	uint8 count1 = 0;
	uint8 count2 = 0;
	SREG |= (1<<7);
	DDRB |= (1<<PB0);
	PORTB &= ~(1<<PB0);
	Uart_config uart_config = {ASYNC,EIGHT_BIT,DISABLED,9600};
	UART_init(&uart_config);
	Timer0_config timer_config = {F_CPU_1024,NORMAL,0} ;
	Timer0_setCallBack_OVR(timer_interrupt);
	uint8 mode;
	DDRA|=0xF3; // configure PA0 & PA1 as output
	PORTA |= 0xFC; //stop motor at first
	while(1){
		//get the mode from other controller
		mode = UART_recieveByte();
		if(mode == '/')
		{
			for(i = 0;i < 4;i++){
				pass[i]=UART_recieveByte();
				_delay_ms(500);
			}
			for(i = 0;i < 4;i++){
				confirm_pass[i]=UART_recieveByte();
				_delay_ms(500);
							}
			if(pass[0] == confirm_pass[0] && pass[1] == confirm_pass[1] && pass[2] == confirm_pass[2] && pass[3] == confirm_pass[3] ){
				UART_sendByte(1);
				PORTA |= 0xFC;
				EEPROM_writeByte(0x0311, pass[0]);
				EEPROM_writeByte(0x0322, pass[1]);
				EEPROM_writeByte(0x0333, pass[2]);
				EEPROM_writeByte(0x0344, pass[3]);

			}
			else{
				PORTA |= 0xFC;
				UART_sendByte(0);
			}
		}
		else if(mode == '#'){
			for(i = 0;i < 3;i++){
				pass[i] = UART_recieveByte();
				_delay_ms(500);
				}
				EEPROM_readByte(0x0311,&confirm_pass[0]);
				EEPROM_readByte(0x0322,&confirm_pass[1]);
				EEPROM_readByte(0x0333,&confirm_pass[2]);
				EEPROM_readByte(0x0344,&confirm_pass[3]);
		/*check if pass sent via UART is the same pass saved in EEPROM*/
			if(pass[0] == confirm_pass[0] && pass[1] == confirm_pass[1] && pass[2] == confirm_pass[2] && pass[3] == confirm_pass[3]){
				/*send 1 through UART to inform other controller that the pass is correct to allow user to reset pass*/
				UART_sendByte(1);
				PORTA |= 0xFC;
				count2 = 0;
			}
			else{
				UART_sendByte(0);
				PORTA |= 0xFC;
				count2++;
			}
			if(count2 >= 3){
					count2 = 0;
					PORTB |= (1<< PB0);
					Timer0_init(&timer_config);
					while(g_buzzer_ticks <= 1836);
					PORTB &= ~(1<< PB0);
					Timer0_stop();
				}

	   }
		else if(mode == '*' ){
			for(i = 0;i < 3;i++){
				pass[i] = UART_recieveByte();
				_delay_ms(500);
				}
				EEPROM_readByte(0x0311,&confirm_pass[0]);
				EEPROM_readByte(0x0322,&confirm_pass[1]);
				EEPROM_readByte(0x0333,&confirm_pass[2]);
				EEPROM_readByte(0x0344,&confirm_pass[3]);

				if(pass[0] == confirm_pass[0] && pass[1] == confirm_pass[1] && pass[2] == confirm_pass[2] && pass[3] == confirm_pass[3]){
					/*then pass sent through UART is the same password saved in EEPROM
					 * we should rotate motor anticlockwise for 10 seconds then rotate it clockwise for 10 seconds*/
					UART_sendByte(1);
					count1 = 0;
					Timer0_init(&timer_config);
					while(g_motor_ticks < 306){
						PORTA &= (~(1<<PA0));
						PORTA |= (1<<PA1);
					}
					while(g_motor_ticks >= 306 && g_motor_ticks <= 612){
						PORTA |= (1<<PA0);
						PORTA &= (~(1<<PA1));
					}
					Timer0_stop();
					PORTA = 0xFC;
				}
				else{
					UART_sendByte(0);
					PORTA |= 0xFC;
					count1++;
				}
				if(count1 >= 3){
					count1 = 0;
					PORTB |= (1<< PB0);
					Timer0_init(&timer_config);
					while(g_buzzer_ticks <= 1836);
					PORTB &= ~(1<< PB0); //stop buzzer
					Timer0_stop();
				}
		}
	}
}




