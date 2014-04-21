#include "config.h"

void uart_init(void)
{
	__REG(UART_PHYS + UCR1) = 0x0;
	__REG(UART_PHYS + UCR2) = 0x0;
	
	while (!(__REG(UART_PHYS + UCR2) & UCR2_SRST));

	// note: the above really works?? I think we have to wait some cycles
	volatile int i;
	for (i=0; i< 10000; i++);
	
	__REG(UART_PHYS + UCR3) = 0x0704;
	__REG(UART_PHYS + UCR4) = 0x8000;
	__REG(UART_PHYS + UESC) = 0x002b;
	__REG(UART_PHYS + UTIM) = 0x0;
	
	__REG(UART_PHYS + UTS) = 0x0;
	
	__REG(UART_PHYS + UFCR) = (4 << 7) | 0x1; /* divide clock by 2 / RxFIFO thresold to 1*/
	__REG(UART_PHYS + UBIR) = 0xf;
	__REG(UART_PHYS + UBMR) = 0x04c4b400 / (2 * 115200);
	
	__REG(UART_PHYS + UCR2) = UCR2_WS | UCR2_IRTS | UCR2_RXEN | UCR2_TXEN | UCR2_SRST;
	
	__REG(UART_PHYS + UCR1) = UCR1_UARTEN | UCR1_RRDYEN;
}

char uart_getc (void)
{
	while (__REG(UART_PHYS + UTS) & UTS_RXEMPTY);
	return (char)(__REG(UART_PHYS + URXD) & URXD_RX_DATA);
}

void uart_putc(char c)
{
	__REG(UART_PHYS + UTXD) = c;

	/* wait for transmitter to be ready */
	while (!(__REG(UART_PHYS + UTS) & UTS_TXEMPTY));

	/* If \n, also do \r */
	if (c == '\n')
		uart_putc('\r');
}

void uart_puts(const char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}