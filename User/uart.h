#ifndef __UART_H
#define __UART_H

#include "gd32f10x.h"

#define U0_RX_SIZE	2048
#define U0_RX_MAX	256
#define URxNUM		10

//uart control blok
typedef struct{
	uint8_t *start;
	uint8_t *end;
}UCB_URxBuffPtr;

typedef struct{
	uint16_t URxCounter;
	UCB_URxBuffPtr URxDataPtr[URxNUM];
	UCB_URxBuffPtr *URxDataIN;
	UCB_URxBuffPtr *URxDataOUT;
	UCB_URxBuffPtr *URxDataEND;
}UCB_CB;

extern UCB_CB U0CB;
extern uint8_t U0_RxBuff[U0_RX_SIZE];

#endif /* __UART_H */
