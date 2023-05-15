#include "gd32f10x.h"
#include "systick.h"
#include "uart.h"
#include "w25q64.h"
#include "fmc.h"

int main(void)
{
	systick_config();
	
	uart0_init(921600);
	w25q64_init();
	
	u0_printf("Everything is OK\r\n");
	
	while(1)
	{
		u0_transmit_test();
	}
}

