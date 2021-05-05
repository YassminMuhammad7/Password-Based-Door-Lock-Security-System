/*
 * Timer.c
 *
 *  Created on: Dec 18, 2020
 *      Author: Yassmin Muhammad
 *      18/12/2020
 *     5:00 AM
 *******************************************************************************/
#include "Timer.h"


/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */

static volatile void (*g_callBackPtr)(void) = NULL_PTR;


/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after get init NORMAL value */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after get CTC value  */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType * Config_Ptr)
{
	SREG  |= (1<<7);    //Enable global interrupts in MC by setting the I-Bit.

	if (Config_Ptr->mode ==NORMAL)
	{
		TCNT0 =  (Config_Ptr->init_NORMAL_value); // Set Timer initial value

		TIMSK |= (1<<TOIE0); // Enable Timer0 Overflow Interrupt

		TCCR0 |= (1<<FOC0); //1. Non PWM mode FOC0=1

		/*
		* insert the required clock value in the first three bits (CS00, CS01 and C02)
		* of TCCR0 Register
		*/

		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);

	}

	else if(Config_Ptr->mode == CTC)
		{
		TCNT0 =  (Config_Ptr->init_NORMAL_value); // Set Timer initial value

		OCR0  = (Config_Ptr->CTC_value); // Set Compare Value

		TIMSK |= (1<<OCIE0); // Enable Timer0 Compare Interrupt

		TCCR0 |= (1<<FOC0); //1. Non PWM mode FOC0=1

		/*
		 * insert the required mode in the Two bits number 3 and 6  (WGM01 and WGM00)
		 * of TCCR0 Register to be CTC mode
		 */

		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->clock);

		/*
		 * insert the required mode in the Two bits number 3 and 6  (WGM01 and WGM00)
		 * of TCCR0 Register to be CTC mode
		 */


		TCCR0 |= (1<<WGM01);
		}
}

/*
 * Description: Function to set the Call Back function address.
 */
void Timer_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}
