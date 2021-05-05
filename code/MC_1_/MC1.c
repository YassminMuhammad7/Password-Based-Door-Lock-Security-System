/*
 * MC1.c
 *
 *  Created on: Dec 18, 2020
 *      Author: Yassmin Muhammad
 *      18/12/2020
 *     5:00 AM
 *******************************************************************************/
#include "keypad.h"
#include "uart.h"
#include "lcd.h"
#include "Timer.h"

#define M2_READY 0x10

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void  Enter_Send_Password()
{
	uint8 password[5];
	uint8 i;


	LCD_goToRowColumn(1,0);

	for(i=0;i<5;i++)

	{
		password[i] = KeyPad_getPressedKey(); //get the pressed key

		if((password[i] <= 9) && (password[i] >= 0))
		{
			LCD_intgerToString(password[i]); /* display the pressed keypad switch */
		}
		else
		{
				LCD_displayCharacter(password[i]); /* display the pressed keypad switch */
		}

		/* printf("%c", '*');*/

		UART_sendByte(password[i]); //send the pressed key to the second MC using uart
		_delay_ms(500);

	}

}

void Enter_confirm_Password()
{
	LCD_clearScreen(); /* clear the LCD display */
	_delay_ms(200);

	LCD_displayStringRowColumn(0,0,"Enter password:");
	_delay_ms(500);

	Enter_Send_Password();

	 _delay_ms(1500);

	LCD_clearScreen(); /* clear the LCD display */
	_delay_ms(200);

	LCD_displayStringRowColumn(0,0,"Confirm password:");
	_delay_ms(500);

	Enter_Send_Password();

	_delay_ms(1500);

	LCD_clearScreen(); /* clear the LCD display */
	_delay_ms(200);
}

uint8 Check_Value_Func()
{
	uint8 check_value;

	check_value = UART_recieveByte(); /* receive the check_value  from the 2nd MC using uart */

return check_value;
}

void Open_Door()
{
	uint8 mode=1;
	uint8 saved_password_value_2nd=0;

	UART_sendByte(mode); //send the mode to the second MC using uart

	LCD_clearScreen(); /* clear the LCD display */
	_delay_ms(200);


	   LCD_displayStringRowColumn(0,0,"Now, You will ");
	   LCD_displayStringRowColumn(1,0,"open the door");

	   _delay_ms(2000);
		LCD_clearScreen(); /* clear the LCD display */
		_delay_ms(200);

	   LCD_displayStringRowColumn(0,0,"Your password:");


	   Enter_Send_Password();/* Enter your password */
	   saved_password_value_2nd=  Check_Value_Func();
	   LCD_intgerToString( saved_password_value_2nd);
	   _delay_ms(200);

		while(saved_password_value_2nd==0)
		{
			LCD_clearScreen(); /* clear the LCD display */
			_delay_ms(200);
			LCD_displayStringRowColumn(0,0,"The password is");
			LCD_displayStringRowColumn(1,0,"not matching :( ");
			_delay_ms(2000);

			LCD_clearScreen(); /* clear the LCD display */
			_delay_ms(200);

		 LCD_displayStringRowColumn(0,0,"Your password:");

			 Enter_Send_Password();
			saved_password_value_2nd=Check_Value_Func();

		}
	   if (saved_password_value_2nd==1){

	  	         LCD_clearScreen(); /* clear the LCD display */
	  	         _delay_ms(200);

	  	         LCD_displayStringRowColumn(0,0,"The door is");
	  	         LCD_displayStringRowColumn(1,0,"opening now :D");
	  		  	 _delay_ms(15000);

	  	         LCD_clearScreen(); /* clear the LCD display */
	  	         _delay_ms(200);

	  	         LCD_displayStringRowColumn(0,0,"The door is");
	  	         LCD_displayStringRowColumn(1,0,"closing now :D");
	  	         _delay_ms(15000);

	   }


}
void Change_Password()
{
	uint8 mode=0 ;
	uint8 saved_password_value_2nd=0;

	UART_sendByte(mode); //send the mode to the second MC using uart

	LCD_clearScreen(); /* clear the LCD display */
	_delay_ms(200);


	   LCD_displayStringRowColumn(0,0,"Now, You will ");
	   LCD_displayStringRowColumn(1,0,"change password");

	   _delay_ms(2000);
		LCD_clearScreen(); /* clear the LCD display */
		_delay_ms(200);

	   LCD_displayStringRowColumn(0,0,"Old password:");


	   Enter_Send_Password();/* Enter the old password */
	   saved_password_value_2nd=  Check_Value_Func();
	   LCD_intgerToString( saved_password_value_2nd);
	   _delay_ms(200);

		while(saved_password_value_2nd==0)
		{
			LCD_clearScreen(); /* clear the LCD display */
					_delay_ms(200);
			LCD_displayStringRowColumn(0,0,"The password is");
			LCD_displayStringRowColumn(1,0,"not matching :( ");
			_delay_ms(2000);

			LCD_clearScreen(); /* clear the LCD display */
			_delay_ms(200);

		 LCD_displayStringRowColumn(0,0,"Old password:");

			 Enter_Send_Password();
			saved_password_value_2nd=Check_Value_Func();

		}
	   if (saved_password_value_2nd==1){

	  	         LCD_clearScreen(); /* clear the LCD display */
	  	          	 	_delay_ms(200);

	  	          	   LCD_displayStringRowColumn(0,0,"New password");
	  	          	   Enter_confirm_Password();/* Enter new password and confirm new password */

	  	          	   LCD_displayStringRowColumn(1,0,"is saved :D");

	  	          		   _delay_ms(2000); }

}

/*****************(Main Function)*************************************************
/****************************************************************************************************************/


int main(void)
{
	uint8 key;
	uint8 mode;
	uint8 check_value_1st_mode;
	uint8 eeprom_read ; /*variable to  receive variable to  show the 1st enter & comfirm password only once forever */

	/* Create configuration structure for Timer driver */
	Timer_ConfigType Timer_Config = {F_CPU_1024,NORMAL,5,250};
	/* Create configuration structure for UART driver */
	UART_ConfigType uart_Config = {ONE_STOP_BIT,ASYNCHRONOUS,DISABLE,ASYNCHRONOUS_DOUBLE_SPEED_MODE};


	UART_init(& uart_Config );	/* Init UART */
	LCD_init();	  /* Init LCD */

/****************************************************************************************************************/
/********(1st interfacing screen with owner-[1st mode] enter password for 1st time)*************************************************/

	UART_sendByte(M2_READY);
		 eeprom_read  = UART_recieveByte(); /* receive  value  to the 2nd MC using uart */

if ( eeprom_read==0xff)
{
	Enter_confirm_Password();
	check_value_1st_mode=Check_Value_Func();

while(check_value_1st_mode==0)
{
	Enter_confirm_Password();
	check_value_1st_mode=Check_Value_Func();

}
if (check_value_1st_mode==1)
{
	LCD_clearScreen(); /* clear the LCD display */
		    _delay_ms(200);

		   LCD_displayStringRowColumn(0,0,"Save password :D");
		   _delay_ms(1000);
	}}

/****************************************************************************************************************/
/******(2st interfacing screen with owner- to select Open Door or Change Password )*************************************************/

    while(1)
    {
    	LCD_clearScreen(); /* clear the LCD display */
    	_delay_ms(200);

 	   LCD_displayStringRowColumn(0,0,"  ***WELCOME*** ");

       	_delay_ms(2000);

    	LCD_clearScreen(); /* clear the LCD display */
    	_delay_ms(200);


	   LCD_displayStringRowColumn(0,0,"Open Door: 1 ");
	   LCD_displayStringRowColumn(1,0,"Change Pass: 2");

		key = KeyPad_getPressedKey(); //get the pressed key

/*****************************( Open Door mode )*************************************************/
		if(key == 1)
		{
			Open_Door();

		}
/*****************************( change password mode )*************************************************/
		else if(key == 2)
		{
			Change_Password();

		}
		else
		{
			   LCD_displayStringRowColumn(0,0,"Wrong pressed");
			   LCD_displayStringRowColumn(1,0,"Try again");
		}

    }
}
