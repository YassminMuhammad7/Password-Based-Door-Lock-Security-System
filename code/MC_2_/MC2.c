/*
 * MC2.c
 *
 *  Created on: Dec 18, 2020
 *      Author: Yassmin Muhammad
 *      18/12/2020
 *     5:00 AM
 *******************************************************************************/
#include "external_eeprom.h"
#include "i2c.h"
#include "uart.h"
#include "lcd.h"
#include "Timer.h"

#define M2_READY 0x10
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 tick = 0;
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

void APP_Timer(void)
{
	tick++;
	if(tick == 60)
	{

		PORTC &= (~(1<<PC2));
		PORTC &= (~(1<<PC3));

	}
	else if(tick == 62)
	{
		PORTC &= (~(1<<PC2));
		PORTC |= (1<<PC3);

	}
	else if(tick == 122)
	{
		PORTC &= (~(1<<PC2));
		PORTC &= (~(1<<PC3));
		TCCR0 = 0;

	}

}

void  Receive_Password( uint8 *password )
{
	uint8 i; /* increment value of for loops */

	for(i=0;i<5;i++)
	{

		*(password + i) = UART_recieveByte(); /* receive the pressed key from uart */

		if( ( *(password + i)  >= 0) && ( *(password + i) <= 9) )
		{
			LCD_intgerToString( *(password + i) ); /* display the pressed keypad switch */
		}
		else
		{
			LCD_displayCharacter( *(password + i)  ); /* display the pressed keypad switch */
		}
	}


}

void  Write_Password( uint8 *enter_password )/*func. to write in the external EEPROM  */
{
	uint8 i; /* increment value of for loops */
	int address= 0x0311; /* the 1st address of EEPROM */


	 _delay_ms(500);
	    	 LCD_goToRowColumn(1,0);/*[start] to make sure from results of func.>> Receive_Password( &password ) */
			for(i=0;i<5;i++)
				{

				LCD_intgerToString( *(enter_password + i) ); /*to show me value on LCD */

		    	_delay_ms(200);
				}/*[end] */

			for(i=0;i<5;i++) /* for loop to  write in the external EEPROM  */
				{
				EEPROM_writeByte(address,*(enter_password + i)); /* Write password[i] (1st enter password) in the external EEPROM */
			    address++;

		    	_delay_ms(100);
				}

	    	 _delay_ms(1000);
	    	   LCD_clearScreen(); /* clear the LCD display and begin from 0 Col, 0 Row */
	    	  _delay_ms(200);

}

void  Read_Password( uint8 *confirm_password, uint8 *enter_password  )/* func. to read from the external EEPROM  */
{
	uint8 i; /* increment value of for loops */
	int address= 0x0311; /* the 1st address of EEPROM */




	 _delay_ms(500);

	    	 LCD_goToRowColumn(1,0);/*[start] to make sure from results of func.>> Receive_Password( &password ) */

			for(i=0;i<5;i++)/* for loop to  read from the external EEPROM  */
				{
				EEPROM_readByte(address, &*(enter_password + i) ); /* Read password[i] (confirm password) from the external EEPROM */
				address++;
				/* to show me value on LCD  */
				LCD_intgerToString( *(enter_password + i) );
						    	_delay_ms(200);

		    	_delay_ms(100);
				}

	    	 _delay_ms(1000);
	    	   LCD_clearScreen(); /* clear the LCD display and begin from 0 Col, 0 Row */
	    	  _delay_ms(200);




}

uint8 Matching_Password(uint8 *enter_password , uint8 *confirm_password)
{
	uint8 check_value;
	uint8 i;

	for(i=0;i<5;i++)
	{
		if(*(enter_password + i) == *(confirm_password + i))
		{
			check_value= 1;

		}
		else
		{
			check_value= 0;
			i= 5;
		}
	}

	return check_value;
}



int main(void)
{ DDRC=0XFF;
	uint8 i; /* increment value of for loops */

	uint8 enter_password[5];/* array to save value of enter password */
	uint8 confirm_password[5];/* array to save value of confirm password  */
	uint8 saved_password[5];/* array to save value of saved password  */

	uint8 show_1st_password_once; /*variable to show the 1st enter & comfirm password only once forever */
	uint8 send_show_1st_password_once; /*variable to  send variable to  show the 1st enter & comfirm password only once forever */


	uint8 check_value_1st_mode;
	uint8 mode;
	uint8 saved_password_value;
	uint8 saved_password_value_2nd;

	uint8 val;/*variable to show the 1st enter & comfirm password only once forever */

	/* Create configuration structure for Timer driver */
	Timer_ConfigType Timer_Config = {F_CPU_1024,NORMAL,5,250};
	/* Create configuration structure for UART driver */
	UART_ConfigType uart_Config = {ONE_STOP_BIT,ASYNCHRONOUS,DISABLE,ASYNCHRONOUS_DOUBLE_SPEED_MODE};

	/* uint8 key;*/
	UART_init(& uart_Config );	/* Init UART */
	LCD_init();	  /* Init LCD */
	EEPROM_init();	/* Init EEPROM */

/*****************************( show the 1st enter & confirm password only once forever )*************************************************/
	EEPROM_readByte( 0x0333, &val);
		while(UART_recieveByte() != M2_READY){} // wait mc1 is ready
		 UART_sendByte(val); // send 1 by UART to mc1

/********(1st interfacing screen with owner-[1st mode] enter password for 1st time)*************************************************
/****************************************************************************************************************/

	while(val==0xff)
	{
	 Receive_Password( &enter_password ); /* call func. to receive 1st enter password */
	 Write_Password(&enter_password);/* call func.  to write in the external EEPROM */

	  Receive_Password( &confirm_password ); /*call func. to receive confirm  password */
	  Read_Password( &confirm_password,&enter_password);/* call func. to read from the external EEPROM */

	  check_value_1st_mode = Matching_Password( &enter_password , &confirm_password);

	  UART_sendByte(check_value_1st_mode); //send the check_value  to the first MC using uart
		 _delay_ms(500);

	  		 while(check_value_1st_mode==0 )
	  		 {	 Receive_Password( &enter_password ); /* call func. to receive 1st enter password */
	  		 Write_Password(&enter_password);/* call func.  to write in the external EEPROM */


	  		  Receive_Password( &confirm_password ); /*call func. to receive confirm  password */
	  		  Read_Password( &confirm_password,&enter_password);/* call func. to read from the external EEPROM */



	  		check_value_1st_mode = Matching_Password( &enter_password , &confirm_password);

	  		  UART_sendByte(check_value_1st_mode); //send the check_value  to the first MC using uart
	  			 _delay_ms(500);
	  		 }
		  		EEPROM_writeByte(0x0333,0x00);
	}
/****************************************************************************************************************/
/******(2st interfacing screen with owner- to select Open Door or Change Password )*************************************************/
	while(1){

		/* emta habd2 f change*/
  		mode = UART_recieveByte(); /* receive the pressed key from uart */
  		/*****************************( Open Door mode )*************************************************/

  		if(mode == 1)
  		{
   		   _delay_ms(2500);

   	    	 Receive_Password( &saved_password ); /* call func. to receive 1st enter password */
   	    	 Read_Password( &saved_password,&enter_password);/* call func. to read from the external EEPROM */

   	    	 saved_password_value = Matching_Password( &enter_password , &saved_password);

   	     	 UART_sendByte(saved_password_value); //send the check_value  to the first MC using uart
   	  		 _delay_ms(500);

   	  		 while (saved_password_value==0) /*the old password is Wrong*/
   	  		 {
   	  	    	 Receive_Password( &saved_password ); /* call func. to receive 1st enter password */
   	  	    	 Read_Password( &saved_password,&enter_password);/* call func. to read from the external EEPROM */

   	  	    	 saved_password_value = Matching_Password( &enter_password , &saved_password);

   	  	     	 UART_sendByte(saved_password_value); //send the check_value  to the first MC using uart
   	  	  		 _delay_ms(500);
   	  	  	if (saved_password_value==1) /*the old password is correct*/
   	  	  	{ /* now, you will open the door (turn on the motor)*/

   				PORTC &= (~(1<<PC3));
   				PORTC |= (1<<PC2);

   	  	  		Timer_init(&Timer_Config);

   	  		/* Set the Call back function pointer in the Timer driver */
   	  		Timer_setCallBack(APP_Timer);


   	  	  	}
   	  	}
		}
 /*****************************( change password mode )*************************************************/

  		else if(mode == 0)
  		 {
  		   _delay_ms(2500);

  	    	 Receive_Password( &saved_password ); /* call func. to receive 1st enter password */
  	    	 Read_Password( &saved_password,&enter_password);/* call func. to read from the external EEPROM */

  	    	 saved_password_value = Matching_Password( &enter_password , &saved_password);

  	     	 UART_sendByte(saved_password_value); //send the check_value  to the first MC using uart
  	  		 _delay_ms(500);

  	  		 while (saved_password_value==0)/*the old password is correct*/
  	  		 {

  	  	    	 Receive_Password( &saved_password ); /* call func. to receive 1st enter password */
  	  	    	 Read_Password( &saved_password,&enter_password);/* call func. to read from the external EEPROM */

  	  	    	 saved_password_value = Matching_Password( &enter_password , &saved_password);

  	  	     	 UART_sendByte(saved_password_value); //send the check_value  to the first MC using uart
  	  	  		 _delay_ms(500);
  	  	  	if (saved_password_value==1)/*the old password is correct*/
  	  	  	{/* now, you will change the password (receive new password )*/

  	  	  		Receive_Password( &saved_password );
  	  	  		Write_Password(&saved_password);
  	  	  	}
  	  	}
		 }

  }
}

