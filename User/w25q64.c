#include "w25q64.h"
#include "spi.h"

void w25q64_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_4);
	CS_DISENABLE;
	spi0_init();
}

void w25q64_waitbusy(void)
{
	uint8_t res;
	do{
		CS_ENABLE;
		spi0_rw_byte(0x55);
		res = spi0_rw_byte(0xff);
		CS_DISENABLE;
	}while((res&0x01) == 0x01);
	
}

void w25q64_enable(void)
{
	w25q64_waitbusy();
	CS_ENABLE;
	spi0_rw_byte(0x06);
	CS_DISENABLE;
}

void w25q64_erase64k(uint8_t blockNB)
{
	uint8_t wdata[4];
	
	wdata[0] = 0xd8;
	wdata[1] = (blockNB*64*1024) >> 16;
	wdata[1] = (blockNB*64*1024) >> 8;
	wdata[1] = (blockNB*64*1024) >> 0;
	
	w25q64_waitbusy();
	w25q64_enable();
	CS_ENABLE;
	spi0_write(wdata,4);
	CS_DISENABLE;
	w25q64_waitbusy();
}

void w25q64_page_write(uint8_t *wbuff,uint16_t pageNB)
{
	uint8_t wdata[4];
	
	wdata[0] = 0x02;
	wdata[1] = (pageNB*256) >> 16;
	wdata[1] = (pageNB*256) >> 8;
	wdata[1] = (pageNB*256) >> 0;
	
	w25q64_waitbusy();
	w25q64_enable();
	CS_ENABLE;
	spi0_write(wdata,4);
	spi0_write(wbuff,256);
	CS_DISENABLE;
}

void w25q64_read(uint8_t *rbuff,uint16_t addr,uint32_t datalen)
{
	uint8_t wdata[4];
	
	wdata[0] = 0x03;
	wdata[1] = (addr) >> 16;
	wdata[1] = (addr) >> 8;
	wdata[1] = (addr) >> 0;
	
	w25q64_waitbusy();
	CS_ENABLE;
	spi0_write(wdata,4);
	spi0_read(rbuff,datalen);
	CS_DISENABLE;
}

