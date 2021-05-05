 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Yassmin Muhammad
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
typedef enum
{
		ONE_STOP_BIT,TWO_STOP_BIT
} UART_STOPBIT;
typedef enum
{
		ASYNCHRONOUS,SYNCHRONOUS
} UART_MODE;
typedef enum
{
		DISABLE,RESERVED,EVEN_PARITY,ODD_PARITY
} UART_PARITYMODE;

typedef enum
{  ASYNCHRONOUS_NORMAL_MODE,ASYNCHRONOUS_DOUBLE_SPEED_MODE

} UART_OPERATION_MODE;

typedef struct
{
	UART_STOPBIT STOP_BIT;
	 UART_MODE MODE;
	 UART_PARITYMODE  PARITY_MODE;
	 UART_OPERATION_MODE OPERATION_MODE;
}UART_ConfigType;



/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* UART Driver Baud Rate */
#define USART_BAUDRATE 9600

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const  UART_ConfigType * Config_Ptr );

void UART_sendByte(const uint8 data);

uint8 UART_recieveByte(void);

void UART_sendString(const uint8 *Str);

void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
