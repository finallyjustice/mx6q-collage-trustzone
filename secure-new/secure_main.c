#include "config.h"

extern u32 monitor_VT;

void delay(void)
{
	u32 i;
	for( i=0; i<0x100000; i++ );
}

void led_ctrl(u32 led_flag)
{
    u32 led_reg;
	delay();

    led_reg = 0x020b4004;
    if( LED_ON == led_flag ){
        __REG(led_reg) = 0x0;
    }
    else {
        __REG(led_reg) = 0x3;
    }
}

void target_init(void)
{
	u32 reg, i;	
	char* dest = (char*)0x27800000;
	char* src  = (char*)0x0090b000;	
	/* For Peripheral access policy, This is may be similar with TZPC function in my opinion
	it can be decide device secure or non-secure on the AXI-APB bus dynamically
	now setting all peripheral can be access in non-secure
	but it will change for secure access restrict, example the internal ram access permit
	*/
	for(reg = 0x021C0000; reg < 0x021C00A0; reg = reg + 4)
		__REG(reg) = 0x00ff00ff; 
	
	// for CORAM secure policy setting
	//reg = 0x021C0000 + 0x**;
	//__REG(reg) = 0x00330033;
	
	// for OCRAM secure status setting
	reg = 0x020e0028;
	__REG(reg) = 0x00003810; // start address it 0, it means all internal ram will be use as secure ram	
	
	// copy uboot from iram to sadram
	for(i=0; i<160096; i++)
		*dest++ = *src++;
		
#if 0	
	//start uboot directly
	(*((void(*)(void))0x27800000))(); 
#endif	
}

void monitor_init(void)
{
	CP15_SET_MVBAR(&monitor_VT);
	
	CP15_SET_SCR(SCR_SETTING);
	
	CP15_SET_NSACR(NSACR_SETTING);
	
	CP15_SET_CPACR(0x00f00000); // Full access for cp11 and cp10
	
	// for initialize target
	target_init();
}

int secure_main(void)
{	
	register unsigned int arg_r0 asm("r0") = ID_RUN_UBOOT;
	register unsigned int arg_r1 asm("r1") = 2;
	register unsigned int arg_r2 asm("r2") = 3;
	register unsigned int arg_r3 asm("r3") = 4;
	
	__asm__ volatile (".arch_extension sec\n\t"                               
						"dsb\n\t"                                               
						"smc #0\n\t"                                            
						: "=r" (arg_r0)                                         
						: "0" (arg_r0), "r" (arg_r1), "r" (arg_r2), "r" (arg_r3)
						: "ip", "lr", "memory", "cc");

	arg_r0 = ID_EXE_CMD;

	while(1) {
		led_ctrl(LED_ON);
		__asm__ volatile (".arch_extension sec\n\t"                               
						"dsb\n\t"                                               
						"smc #0\n\t"                                            
						: "=r" (arg_r0)                                         
						: "0" (arg_r0), "r" (arg_r1), "r" (arg_r2), "r" (arg_r3)
						: "ip", "lr", "memory", "cc");
		//asm volatile ("smc #0\n\t");		
	}
	
	return 0;
}
