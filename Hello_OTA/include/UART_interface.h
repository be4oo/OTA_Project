/********************************************************************************/
/* Author  : BESHO																*/
/* Version : V01																*/
/* Date    : 19 SEP 2020														*/
/********************************************************************************/


#ifndef UART_INTERFACE_H
#define UART_INTERFACE_H

void MUART_voidInit(void);

void MUART_voidTransmit(u8 arr[]);

u8 MUART_u8Receive(u32 EchoTime);

u8 MUSART1_u8Receive(u8* Copy_u8Data);


#endif
