#ifndef __SPI_H
#define __SPI_H

#include "gd32f10x.h"

void spi0_init(void);
uint8_t spi0_rw_byte(uint8_t txd);
void spi0_write(uint8_t *wdata,uint16_t datalen);
void spi0_read(uint8_t *rdata,uint16_t datalen);

#endif /* __SPI_H */
