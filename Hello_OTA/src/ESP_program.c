/********************************************************************************/
/* Author  : BESHO																*/
/* Version : V01																*/
/* Date    : 28 SEP 2020														*/
/********************************************************************************/

#include "STD_TYPES.h"
#include "BIT_MATH.h"


#include "UART_interface.h"

#include "ESP_interface.h"
#include "ESP_private.h"
#include "ESP_config.h"



u8 Local_u8Response[100] = {0};
volatile u8 Local_u8ResponseHEX[14000] = {0};

void ESP_voidInit(void)
{
	u8 Local_u8ValidFlag = 0;

	while(Local_u8ValidFlag == 0)
	{
		/*Stop Echo*/
		MUART_voidTransmit("ATE0\r\n");
		/*	Check command is done right
			ATE0 returns OK if all is okay*/
		Local_u8ValidFlag = u8ValidateCmd(150);
	}
	
	Local_u8ValidFlag = 0;

	while(Local_u8ValidFlag == 0)
	{
		/*Set ESP station mode*/
		MUART_voidTransmit("AT+CWMODE=1\r\n");
		/*	Check command is done right
			AT+CWMODE=1 returns OK if all is okay*/
		Local_u8ValidFlag = u8ValidateCmd(150);
	}
}


u8 u8ValidateCmd(u32 EchoTime)
{
	u8 Local_u8ValidFlag = 0;
	u8 Counter = 0;
	u8 Temp_char = 0;

	for(u8 i = 0; i < 100; i++)
	{
		Local_u8Response[i] = 0;
	}

	/*Make sure that the MC is not receiving any more data from the ESP*/
	while (Temp_char != 255)
	{
		Temp_char = MUART_u8Receive(EchoTime);
		Local_u8Response[Counter] = Temp_char;
		Counter++;
	}

	/*	Deleting last element as it is always 255 for stop receiving data	*/
	Local_u8Response[Counter-1] = 0;

	/*Strcmp returns 0 if the two strings are iqentical*/
	//Local_u8ValidFlag = strcmp(Copy_u8StrCmd, Local_u8Response);

	/* Checks if any of the received data contains the word "OK" meaning the command is done successfully*/
	for(u8 i = 0; i < Counter - 2; i ++)
	{
		if(Local_u8Response[i] == 'O' && Local_u8Response[i+1] == 'K')
		{
			Local_u8ValidFlag = 1;
		}
	}
	return Local_u8ValidFlag;
}
u8 u8ValidateCmd1(u32 EchoTime)
{
	u8 Local_u8ValidFlag = 0;
	u16 Counter = 0;
	u8 Temp_char = 0;

	/*Make sure that the MC is not receiving any more data from the ESP*/
	while (Temp_char != 255)
	{
		Temp_char = MUART_u8Receive(EchoTime);
		Local_u8ResponseHEX[Counter] = Temp_char;
		Counter++;
	}

	/*	Deleting last element as it is always 255 for stop receiving data	*/
	Local_u8ResponseHEX[Counter-1] = 0;

	/*Strcmp returns 0 if the two strings are iqentical*/
	//Local_u8ValidFlag = strcmp(Copy_u8StrCmd, Local_u8Response);

	/* Checks if any of the received data contains the word "OK" meaning the command is done successfully*/
	for(u16 i = 0; i < Counter - 2; i ++)
	{
		if(Local_u8ResponseHEX[i] == 'O' && Local_u8ResponseHEX[i+1] == 'K')
		{
			Local_u8ValidFlag = 1;
		}
	}
	return Local_u8ValidFlag;
}


void ESP_WIFIConnect(void)
{
	u8 Local_u8Result = 0;

	while(Local_u8Result == 0)
	{
		/* stop ECHO */
		MUART_voidTransmit("AT+CWJAP_CUR=\"sasa\",\"qwe@123#qwe123@#\"\r\n");
		Local_u8Result = u8ValidateCmd(10000);
	}
}

void ESP_voidSendData(void)
{
	u8 Local_u8Result = 0;

	while(Local_u8Result == 0)
	{
		/* set IP */
		MUART_voidTransmit("AT+CIPSTART=\"TCP\",\"162.253.155.226\",80\r\n");
		Local_u8Result = u8ValidateCmd(30000);
	}

	Local_u8Result = 0;


}

u8 *ESP_u8WebState(void)
{
	u8 i = 0;
	u8 Local_u8Result = 0;

	while(Local_u8Result == 0)
	{
		/* Send */
		MUART_voidTransmit("AT+CIPSEND=45\r\n");
		Local_u8Result = u8ValidateCmd(10000);
	}
	Local_u8Result = 0;
		/* get state */
	while(Local_u8Result == 0)
	{
		/* Send */
		MUART_voidTransmit("GET http://besho.freevar.com/status.txt\r\n");
		Local_u8Result = u8ValidateCmd1(900000);
	}
	Local_u8Result = 0;


	//static u8 L[5] = {1,2,3,4,5};

	return Local_u8ResponseHEX;
}

