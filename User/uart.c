#include "uart.h"

uint8_t U0_RxBuff[U0_RX_SIZE];
uint8_t U0_TxBuff[U0_TX_SIZE];
UCB_CB U0CB;

static void u0rx_ptr_init(void)
{
	U0CB.URxDataIN = &U0CB.URxDataPtr[0];
	U0CB.URxDataOUT = &U0CB.URxDataPtr[0];
	U0CB.URxDataEND = &U0CB.URxDataPtr[URxNUM-1];
	U0CB.URxDataIN->start = U0_RxBuff;
	U0CB.URxCounter = 0;
}

static void uart0_dma_init(void)
{
	dma_parameter_struct Dma_Init_Sturt;
	
	rcu_periph_clock_enable(RCU_DMA0);
	
	dma_deinit(DMA0,DMA_CH4);
	
	Dma_Init_Sturt.periph_addr = USART0+4;
	Dma_Init_Sturt.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
	Dma_Init_Sturt.memory_addr = (uint32_t)U0_RxBuff;
	Dma_Init_Sturt.memory_width = DMA_MEMORY_WIDTH_8BIT;
	Dma_Init_Sturt.number = U0_RX_MAX+1;
	Dma_Init_Sturt.priority = DMA_PRIORITY_HIGH;
	Dma_Init_Sturt.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
	Dma_Init_Sturt.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
	Dma_Init_Sturt.direction = DMA_PERIPHERAL_TO_MEMORY;
	
	dma_init(DMA0,DMA_CH4,&Dma_Init_Sturt);
	
	dma_circulation_disable(DMA0,DMA_CH4);
	
	dma_channel_enable(DMA0,DMA_CH4);
}

void uart0_init(uint32_t bandrate)
{
	rcu_periph_clock_enable(RCU_USART0);
	rcu_periph_clock_enable(RCU_GPIOA);
	
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_10);
	
	usart_deinit(USART0);
	usart_baudrate_set(USART0,bandrate);
	usart_parity_config(USART0,USART_PM_NONE);
	usart_word_length_set(USART0,USART_WL_8BIT);
	usart_stop_bit_set(USART0,USART_STB_1BIT);
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);
	usart_dma_receive_config(USART0,USART_DENR_ENABLE);
	
	usart_interrupt_enable(USART0,USART_INT_IDLE);
	
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
	nvic_irq_enable(USART0_IRQn,0,0);
	
	usart_enable(USART0);
	
	u0rx_ptr_init();
	uart0_dma_init();
}

void u0_transmit_test(void)
{
	if(U0CB.URxDataOUT != U0CB.URxDataIN){
		u0_printf("resevie %d byte\r\n",U0CB.URxDataOUT->end - U0CB.URxDataOUT->start + 1);
		for(int i=0; i < U0CB.URxDataOUT->end - U0CB.URxDataOUT->start + 1; i++){
			u0_printf("%c",U0CB.URxDataOUT->start[i]);
		}
		u0_printf("\r\n");
		
		U0CB.URxDataOUT++;
		if(U0CB.URxDataOUT == U0CB.URxDataEND){
			U0CB.URxDataOUT = &U0CB.URxDataPtr[0];
		}
	}

}

void u0_printf(char *format,...)
{
	uint16_t i;
	va_list listdata;
	va_start(listdata,format);
	
	vsprintf((char *)U0_TxBuff,format,listdata);
	
	va_end(listdata);
	
	for(i=0;i<strlen((const char*)U0_TxBuff);i++){
		while(usart_flag_get(USART0,USART_FLAG_TBE) != 1);//等待发送缓冲区为0
		usart_data_transmit(USART0,U0_TxBuff[i]);
	}
	
	while(usart_flag_get(USART0,USART_FLAG_TBE) != 1);
}
