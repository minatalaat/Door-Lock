/*
 * timer.c
 *
 *  Created on: Oct 4, 2020
 *      Author: mina talaat
 */
#include "timer.h"
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
/*Global pointer to hold call back pointer of overflow interrupt*/
static volatile void (*g_callBack_OVR_Ptr)(void) = NULL_PTR;
/*Global pointer to hold call back pointer of CompareMatch interrupt*/
static volatile void (*g_callBack_COMP_Ptr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
ISR(TIMER0_OVF_vect){
	if(g_callBack_OVR_Ptr != NULL_PTR)
		{
			(*g_callBack_OVR_Ptr)();
		}
}
ISR(TIMERO_COMP_vect){
	if(g_callBack_COMP_Ptr != NULL_PTR)
			{
				(*g_callBack_COMP_Ptr)();
			}
}

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*set Timer0 mode
 * set Timer0 clock
 * set initial value of timer0
 * set CompareValue if needed */
void Timer0_init(const Timer0_config * a_Config_Ptr){
	if(a_Config_Ptr -> mode == NORMAL){
		TCCR0 &= ~(1<<WGM00) & ~(1<<WGM01); //set timer in normal mode
		/* set timer Clock*/
		TCCR0 =(TCCR0 & 0xF8) | (a_Config_Ptr -> clk);
		/*setting initial value*/
		TCNT0 = a_Config_Ptr -> initial_value;
		/*enable overflow interrupt*/
		TIMSK |= (1<<TOIE0);
		TCCR0 |= (1<<FOC0);
	}
	else if(a_Config_Ptr -> mode == COMPARE){
		/* set timer in compare*/
		TCCR0 |= (1<<WGM01);
		TCCR0 &= ~(1<<WGM00);
		/* set timer Clock*/
		TCCR0 =(TCCR0 & 0xF8) | (a_Config_Ptr -> clk);
		/* enable Compare match interrupt*/
		TIMSK |= (1<<OCIE0);
		TCCR0 |= (1<<FOC0);
		/*set compare value*/
		OCR0 = a_Config_Ptr->compare_value;
	}

}
void Timer0_stop(void){
	TCCR0 = 0;
}
void Timer0_setCallBack_OVR(void(*a_OVR_ptr)(void)){
	g_callBack_OVR_Ptr = a_OVR_ptr;
}

void Timer0_setCallBack_COMP(void(*a_COMP_ptr)(void)){
	g_callBack_COMP_Ptr = a_COMP_ptr;
}

