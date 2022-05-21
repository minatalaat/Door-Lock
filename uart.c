/*
 * uart.c
 *
 *  Created on: Oct 5, 2020
 *      Author: mina talaat
 */
#include "uart.h"

#define BAUD_PRESCALE (((8000000 / (9600 * 8UL))) - 1)

void UART_init(const Uart_config * a_Config_Ptr ){
	UCSRA = (1<<U2X);
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For all Char size we use this bit is cleared
	 * RXB8 & TXB8 not used
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	UCSRC |= (1<<URSEL);
	//set UART_mode (ASYNC or SYNC)
	UCSRC = (UCSRC & 0xBF) | ((a_Config_Ptr->mode)<< 6);
	UCSRC = (UCSRC & 0xCF) | ((a_Config_Ptr->ParityMode)<<4);
	UCSRC &= ~(1<<USBS); //one stop bit
	UCSRC = (UCSRC & 0xF9) | ((a_Config_Ptr->charSize)<<1 );
	UCSRC &= ~(1<<UCPOL);
	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;

}
void UART_sendByte(const uint8 data)
{
	/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}
	/* Put the required data in the UDR register and it also clear the UDRE flag as
	 * the UDR register is not empty now */
	UDR = data;
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	*******************************************************************/
}

uint8 UART_recieveByte(void)
{
	/* RXC flag is set when the UART receive data so wait until this
	 * flag is set to one */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}
	/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
    return UDR;
}

void UART_sendString(const uint8 *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

void UART_receiveString(uint8 *Str)
{
	uint8 i = 0;
	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}


