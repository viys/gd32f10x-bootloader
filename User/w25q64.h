#ifndef __W25Q64_H
#define __W25Q64_H

#include "gd32f10x.h"

#define CS_ENABLE 		gpio_bit_reset(GPIOA,GPIO_PIN_4)
#define CS_DISENABLE 	gpio_bit_set(GPIOA,GPIO_PIN_4)

void w25q64_init(void);
void w25q64_erase64k(uint8_t blockNB);
void w25q64_page_write(uint8_t *wbuff,uint16_t pageNB);
void w25q64_read(uint8_t *rbuff,uint16_t addr,uint32_t datalen);

#endif /* __W25Q64_H */
