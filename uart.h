/*
 * uart.h
 *
 *  Created on: Oct 5, 2020
 *      Author: mina talaat
 */

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

typedef enum
{
	FIVE_BIT, SIX_BIT, SEVEN_BIT, EIGHT_BIT

}Uart_Char_size;

typedef enum
{
	ASYNC,SYNC
}Uart_mode;

typedef enum
{
	DISABLED = 0, EVEN_PARITY = 2, ODD_PARITY = 3
}Uart_Parity_mode;

typedef struct
{
	Uart_mode mode;
	Uart_Char_size charSize;
	Uart_Parity_mode ParityMode;
	uint8 Baud_rate;
}Uart_config;

void UART_init(const Uart_config * a_Config_Ptr );

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
