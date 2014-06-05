#include "config.h"

int mxc_iomux_v3_setup_pad(u32 base,iomux_v3_cfg_t pad)
{
	u32 mux_ctrl_ofs = (pad & MUX_CTRL_OFS_MASK) >> MUX_CTRL_OFS_SHIFT;
	u32 mux_mode = (pad & MUX_MODE_MASK) >> MUX_MODE_SHIFT;
	u32 sel_input_ofs =
		(pad & MUX_SEL_INPUT_OFS_MASK) >> MUX_SEL_INPUT_OFS_SHIFT;
	u32 sel_input =
		(pad & MUX_SEL_INPUT_MASK) >> MUX_SEL_INPUT_SHIFT;
	u32 pad_ctrl_ofs =
		(pad & MUX_PAD_CTRL_OFS_MASK) >> MUX_PAD_CTRL_OFS_SHIFT;
	u32 pad_ctrl = (pad & MUX_PAD_CTRL_MASK) >> MUX_PAD_CTRL_SHIFT;

	if (mux_ctrl_ofs)
		REG_WR(base, mux_ctrl_ofs, mux_mode);

	if (sel_input_ofs)
		REG_WR(base, sel_input_ofs, sel_input);

	if (!(pad_ctrl & NO_PAD_CTRL) && pad_ctrl_ofs) {
		if (pad_ctrl & PAD_CTL_LVE) {
			/* Set the bit for LVE */
			pad_ctrl |= (1 << PAD_CTL_LVE_OFFSET);
			pad_ctrl &= ~(PAD_CTL_LVE);
		}
		REG_WR(base, pad_ctrl_ofs, pad_ctrl);
	}

	return 0;
}

void cpu_init(void)
{
	u32 val, reg;

	/* Init AIPSTZ1 & AIPSTZ2 */
	__REG(0x0207c000)= 0x77777777;
	__REG(0x0207c004)= 0x77777777;
	__REG(0x0207c040)= 0x0;
	__REG(0x0207c044)= 0x0;
	__REG(0x0207c048)= 0x0;
	__REG(0x0207c04c)= 0x0;
	__REG(0x0207c050)= 0x0;

	__REG(0x0217c000)= 0x77777777;
	__REG(0x0217c004)= 0x77777777;
	__REG(0x0217c040)= 0x0;
	__REG(0x0217c044)= 0x0;
	__REG(0x0217c048)= 0x0;
	__REG(0x0217c04c)= 0x0;
	__REG(0x0217c050)= 0x0;

	/* Init_clock Restore the default values in the Gate registers */
	REG_WR(0x020c4000, 0x68, 0xC0003F);
	REG_WR(0x020c4000, 0x6c, 0x30FC00);
	REG_WR(0x020c4000, 0x70, 0xFFFC000);
	REG_WR(0x020c4000, 0x74, 0x3FF00000);
	REG_WR(0x020c4000, 0x78, 0xFFF300);
	REG_WR(0x020c4000, 0x7c, 0xF0000C3);
	REG_WR(0x020c4000, 0x80, 0x3FC);
		
	/* Increase the VDDSOC to 1.2V */
	val = REG_RD(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE);
	val &= ~BM_ANADIG_REG_CORE_REG2_TRG;
	val |= BF_ANADIG_REG_CORE_REG2_TRG(0x14);
	REG_WR(ANATOP_BASE_ADDR, HW_ANADIG_REG_CORE, val);

	/* Need to power down PCIe */
	val = REG_RD(IOMUXC_BASE_ADDR, IOMUXC_GPR1_OFFSET);
	val |= (0x1 << 18);
	REG_WR(IOMUXC_BASE_ADDR, IOMUXC_GPR1_OFFSET, val);

	REG_WR(SNVS_BASE_ADDR, 0x64, 0x41736166);/*set LPPGDR*/
	reg = REG_RD(SNVS_BASE_ADDR, 0x4c);
	reg |= (1 << 3);
	REG_WR(SNVS_BASE_ADDR, 0x4c, reg);/*clear LPSR*/

	/* UART1 TXD */
	mxc_iomux_v3_setup_pad(IOMUXC_BASE_ADDR, MX6Q_PAD_SD3_DAT7__UART1_TXD);
	/* UART1 RXD */
	mxc_iomux_v3_setup_pad(IOMUXC_BASE_ADDR, MX6Q_PAD_SD3_DAT6__UART1_RXD);
}

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