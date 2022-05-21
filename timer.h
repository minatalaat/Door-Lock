/*
 * timer.h
 *
 *  Created on: Oct 4, 2020
 *      Author: mina talaat
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	NORMAL = 0, COMPARE = 2
}Timer_Mode;

typedef struct
{
	Timer_Clock clk;
	Timer_Mode mode;
	uint8 initial_value;
	uint8 compare_value;
}Timer0_config;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*initiate timer 0 */
void Timer0_init(const Timer0_config * a_Config_Ptr);
/*stop timer*/
void Timer0_stop(void);
/*set call back for overflow mode*/
void Timer0_setCallBack_OVR(void(*a_OVR_ptr)(void));
/*set call back for Compare mode*/
void Timer0_setCallBack_COMP(void(*a_COMP_ptr)(void));

#endif /* TIMER_H_ */
