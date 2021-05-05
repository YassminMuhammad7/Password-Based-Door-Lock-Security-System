/*
 * Timer.h
 *
 *  Created on: Dec 18, 2020
 *      Author: Yassmin Muhammad
 *      18/12/2020
 *     5:00 AM
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
/*typedef enum
{
	TIMER0,TIMER1,TIMER2
}Timer_Type;
*/

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer_Clock;

typedef enum
{
	NORMAL,CTC
}Timer_Mode;


typedef struct
{
	//Timer_Type type;
	Timer_Clock clock;
	Timer_Mode mode;
	uint8 init_NORMAL_value;
	uint8 CTC_value;
}Timer_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description : Function to initialize the TIMER driver
 * 	1. Set the required mode.
 * 	2. Set the Timer initial value
 * 	3. Set Compare Value in CTC mode only
 * 	4. Set the required clock.
 * 	5. Enable the Input Capture Interrupt.
 */

void Timer_init(const Timer_ConfigType * Config_Ptr);

/*
 * Description: Function to set the Call Back function address.
 */

void Timer_setCallBack(void(*a_ptr)(void));

#endif /* TIMER_H_ */
