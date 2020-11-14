/*
 * main.c
 *
 *  Created on: Oct 13, 2020
 *      Author: besho
 */


#include "STD_TYPES.h"
#include "BIT_MATH.h"

#include "RCC_interface.h"
#include "DIO_int.h"
#include "STK_interface.h"
#include "UART_interface.h"
#include "FPEC_interface.h"
#include "ESP_interface.h"

void Get_Record(void);

volatile u8  u8RecBuffer[100]   ;
volatile u8  u8RecCounter    = 0;
volatile u8  u8TimeOutFlag   = 0;
volatile u8  u8BLWriteReq    = 1;

/* var for control and clean the record */
volatile u8  *HEXFile;
volatile u32 line = 0;
u8 ipd = 1;
volatile u8 bufcount = 0;
volatile u8 last = 0;

typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

void func(void)
{
#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	SCB_VTOR = 0x08001000;

	addr_to_call = *(Function_t*)(0x08001004);
	addr_to_call();
}


void main(void)
{
	RCC_voidInitSysClock(); /* HSI */

	/* ESP ENABLE */

	RCC_voidEnableClock(RCC_APB2,14); /* USART1 */
	RCC_voidEnableClock(RCC_APB2,2);  /* PortA  */
	RCC_voidEnableClock(RCC_AHB,4);   /* FPEC   */


	/* A9 	==> TX out 50 MHz */ /* to RX of ESP */
	MGPIO_voidSetPinDirection(GPIOA, PIN9, OUTPUT_SPEED_50MHZ_AFPP);
	/* A10 	==> RX input floating */ /* to TX of ESP */
	MGPIO_voidSetPinDirection(GPIOA, PIN10, INPUT_FLOATING);

	MGPIO_voidSetPinDirection(GPIOA, PIN0, OUTPUT_SPEED_2MHZ_PP);


	MUART_voidInit(); /* 115200 */

	MSTK_voidInit();

	ESP_voidInit();
	ESP_WIFIConnect();

	ESP_voidSendData();

	/* getting the uploaded file array on HEXFile pointer */
	HEXFile = ESP_u8WebState();

	/* to know your start point of the record */
	if(ipd == 1)
	{
		for(u8 i = 0 ; i < 200 ; i++)
		{
			if(*(HEXFile+i) == 'I' && *(HEXFile+(i+1)) == 'P' && *(HEXFile+(i+2)) == 'D')
			{
				ipd = 0;
				line = i+9;
				break;
			}
		}
	}


	while(1)
	{

		Get_Record();

				if (u8BLWriteReq == 1)
				{
					 FPEC_voidEraseAppArea();
					u8BLWriteReq = 0;
				}

				/* Parse */
				Parser_voidParseRecord(u8RecBuffer);
				bufcount = 0;


				if(last == 1)
				{
					func();
				}

	}
}



void Get_Record(void)
{
	for(u16 j = line ; j < 25000 ; j++)
	{
		if(*(HEXFile+j) == '\n' && *(HEXFile+(j+1)) != '+')
		{
			line = j+1;
			while(bufcount !=99)
			{
				u8RecBuffer[bufcount] = 0;
				bufcount++;
			}
			break;
		}
		/*
		else if(*(HEXFile+j) == ':')
		{
			continue;
		}
		*/
		else
		{
			if(*(HEXFile+(j)) == '+' && *(HEXFile+(j+1)) == 'I')
			{
				j = j + 9;
				bufcount = bufcount - 2;
				continue;
			}
			else
			{
				if(*(HEXFile+(j)) == 'C' && *(HEXFile+(j+1)) == 'L' && *(HEXFile+(j+2)) == 'O' && *(HEXFile+(j+3)) == 'S')
				{
					last = 1;
					break;
				}

				u8RecBuffer[bufcount] = *(HEXFile+(j));
				bufcount++;

			}
		}
	}


}

