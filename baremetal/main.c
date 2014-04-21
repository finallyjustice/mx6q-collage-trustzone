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
	} 
}

int main()
{	
	uart_init();
	uart_puts("\n\rtest uart init\n\r");
	
	enable_hwfirewall() ; 
 
	monitorInit(Normal_World) ; 		// to normal World
	
	while(1) { 	    
		led_on();
		uart_puts("hello from Secure world\n\r");
		asm volatile ("smc #0\n\t") ; 
	};
	
	return 0;
}

