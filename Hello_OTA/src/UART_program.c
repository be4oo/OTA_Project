/********************************************************************************/
/* Author  : BESHO																*/
/* Version : V01																*/
/* Date    : 19 SEP 2020														*/
/********************************************************************************/


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "UART_interface.h"
#include "UART_private.h"
#include "UART_config.h"


void	MUART_voidInit(void)
{
	/* 9600 	==> BRR = 0x341 */
	//MUART->BRR = 0x341;
	/* 115200	==> BRR = 0x45*/
	MUART->BRR = 0x45;
	/*
		1- Enable RX
		2- Enable TX
		3- Enable UART
	*/
	SET_BIT(MUART->CR1,2);
	SET_BIT(MUART->CR1,3);
	SET_BIT(MUART->CR1,13);
	/* Clear Status Registers */
	MUART->SR = 0;
}

void	MUART_voidTransmit(u8 * arr)
{
	u8 i = 0;
	while(arr[i] != '\0')
	{
		MUART->DR = arr[i];
		while(GET_BIT(MUART->SR,6) == 0);
		/* wait till transmision is complete */
		//while(((MUART->SR) & 0x40) == 0);
		i++;
	}
}


u8 MUART_u8Receive(u32 EchoTime)
{
	u32 timeout = 0;
	u8 Local_u8ReceivedData = 0;

	while( (GET_BIT((MUART -> SR), 5) ) == 0)
	{
		timeout++;
		if (timeout == EchoTime)
		{
			Local_u8ReceivedData = 255;
			break;
		}
	}


	if(Local_u8ReceivedData == 0)
	{
		Local_u8ReceivedData = MUART -> DR;
	}
	return (Local_u8ReceivedData);
}

u8 MUSART1_u8Receive(u8* Copy_u8Data)
{
	u8 Local_u8Result = 1;

	if ( (GET_BIT((MUART -> SR), 5)) == 1 )
	{
		*Copy_u8Data = MUART -> DR;
	}
	else
	{
		Local_u8Result = 0;
	}

	return (Local_u8Result);
}

/*
u8		MUART_u8Receive(void)
{
	u32 timeout = 0;
	u8 Local_u8Received = 0;

	while((GET_BIT((MUART->SR), 5)) == 0)
	{
		timeout++;
		if(timeout == 100)
		{
			Local_u8Received = 255;
			break;
		}
	}

	if(Local_u8Received == 0)
	{
		Local_u8Received = (MUART->DR);
		//Local_u8Received = (0xFF & (MUART->DR));
	}
	return (Local_u8Received);
}
*/
