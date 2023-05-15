#ifndef __UART_H
#define __UART_H

#include "gd32f10x.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

#define U0_RX_SIZE	2048
#define U0_TX_SIZE	2048
#define U0_RX_MAX	256
#define URxNUM		10

/* uart control blok */
typedef struct{
	uint8_t *start;
	uint8_t *end;
}UCB_URxBuffPtr;

/* 串口控制区指针结构体 */
typedef struct{
	uint16_t URxCounter;
	UCB_URxBuffPtr URxDataPtr[URxNUM];
	UCB_URxBuffPtr *URxDataIN;
	UCB_URxBuffPtr *URxDataOUT;
	UCB_URxBuffPtr *URxDataEND;
}UCB_CB;

extern UCB_CB U0CB;
extern uint8_t U0_RxBuff[U0_RX_SIZE];

/* BootLoader 串口初始化 */
/* uart0外设初始化 */
void uart0_init(uint32_t bandrate);
/* uart0对应的DMA0初始化 */
void uart0_dma_init(void);
/* uart0指针初始化 */
void u0rx_ptr_init(void);
/* uart0 printf重定向 */
void u0_printf(char *format,...);
/* uart0 transmit测试函数 */
void u0_transmit_test(void);
	
#endif /* __UART_H */
