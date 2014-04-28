#include "config.h"

extern void uart_init(void);
extern void uart_puts(const char *s);
extern void monitorInit();

void enable_hwfirewall(void)
{
	/* Central Security Unit (CSU) */
	unsigned int csu_reg;
	
	for(csu_reg = 0x021C0000; csu_reg < 0x021C00A0; csu_reg = csu_reg + 4)
		__REG(csu_reg) = 0x00ff00ff; //0x00330033 for Peripheral access policy

#if 0		
	// setting OCRAM_TZ_ADDR & OCRAM_TZ_EN to ensure secure 
	csu_reg = 0x020e0028;
	__REG(csu_reg) = 0x00003810;
#endif	
} 

void led_on()
{
	unsigned int csu_reg, i;
	
	for(i=0; i<0x100000; i++); 
	
	csu_reg = 0x020b4004;
	__REG(csu_reg) = 0x0;
}

void led_off()
{
	unsigned int csu_reg, i;
	
	for(i=0; i<0x100000; i++);
	
	csu_reg = 0x020b4004;
	__REG(csu_reg) = 0x3;
}

void Normal_World()
{
	while (1) { 
		led_off();
		uart_puts("hello from Normal world\n\r");
		asm volatile ("smc #0\n\t") ; 
		//(*((void(*)(void))0x27800050))();
	} 
}

void (*gouboot)(void) = 0x27800050;

#define IOMUXC_GPR1_OFFSET	0x4
void cpu_init()
{
	int val;
	unsigned int reg;

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

	mxc_iomux_v3_init((void *)IOMUXC_BASE_ADDR);
	/* UART1 TXD */
	mxc_iomux_v3_setup_pad(MX6Q_PAD_SD3_DAT7__UART1_TXD);
	/* UART1 RXD */
	mxc_iomux_v3_setup_pad(MX6Q_PAD_SD3_DAT6__UART1_RXD);
}

int main()
{	
	unsigned int i;
	char* dest = (char*)0x27800000;
	char* src  = (char*)0x0090C000;
	
	cpu_init();
	
	uart_init();
	uart_puts("\n\renter the bare metal program\n\r");


	for(i=0; i<160098; i++)
		*dest++ = *src++;
	
	enable_hwfirewall() ; 
	
//	gouboot();
 
	monitorInit(Normal_World) ; 		// to normal World
	
	while(1) { 	    
		led_on();
		uart_puts("hello from Secure world\n\r");
		asm volatile ("smc #0\n\t") ; 
	};
	
	return 0;
}

