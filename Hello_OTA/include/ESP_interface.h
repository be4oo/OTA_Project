/********************************************************************************/
/* Author  : BESHO																*/
/* Version : V01																*/
/* Date    : 28 SEP 2020														*/
/********************************************************************************/


#ifndef ESP_INTERFACE_H
#define ESP_INTERFACE_H

void ESP_voidInit(void);

u8 u8ValidateCmd(u32 EchoTime);

u8 u8ValidateCmd1(u32 EchoTime);

void ESP_WIFIConnect(void);

void ESP_voidSendData(void);

u8 *ESP_u8WebState(void);

#endif
