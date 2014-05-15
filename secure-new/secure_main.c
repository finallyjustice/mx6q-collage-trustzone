#include "config.h"

extern u32 monitor_vetor_table;

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

// Central Security Unit
void target_csu_init(void)
{
	u32 reg;
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
	// start address it 0, it means all internal ram will be use as secure ram	
	reg = 0x020e0028;
	__REG(reg) = 0x00003810; 
}

// Snoop Control Unit
void target_scu_init(void)
{
	u32 reg;
	
	// SCU Access Control Register, enable all cpu
	reg = 0x00a00050;
	__REG(reg) = 0xf; 
	
	// SCU Non-secure Access Control Register, non-secure can access
	reg = 0x00a00054;
	__REG(reg) = 0xfff; 
}

// GIC interrupt control unit
#define ICDDCR		0x00a01000
#define ICDISR0		0x00a01080
#define ICDISRnS	0x00a01084
#define ICDISRnE	0x00a01094
#define ICDSGIR		0x00a01f00
#define ICCICR		0x00a00100
#define ICCBPR		0x00a00104
void target_gic_init(void)
{
	u32 reg;
	
	// ICDISR Secure Interrupt Security Registers
	// cortex-a9 only use PPI(31-27), so setting PPI and SGI non-secure
	reg = ICDISR0;
	__REG(reg) = 0xf800ffff;
	
	// set all SPI non-secure, depend on Interrupt Controller Type Register (ICDICTR)ITLinesNumber
	for(reg = ICDISRnS; reg < ICDISRnE; reg = reg + 4)
		__REG(reg) = 0xffffffff; 

#if 1		
	// ICDDCR Banked Distributor Control Register
	reg = ICDDCR;
	__REG(reg) = 0x3;
	
	reg = ICDSGIR;
	__REG(reg) = 1<<15;
#endif	
	// ICCBPR Banked Binary Point Register, this bit must setting in secure status in my opinion
	reg = ICCBPR;
	__REG(reg) = 0xf8;
#if 1	
	// ICCICR Banked CPU Interface Control Register
	reg = ICCICR;
	__REG(reg) = 0xf;
#endif	
}

void target_init(void)
{
	u32 reg, i;	
	char* dest = (char*)0x27800000;
	char* src  = (char*)0x0090b000;	
	
	target_csu_init();
	
	target_scu_init();
	
	target_gic_init();
	
	// L2_Cache setting
	reg = 0x00a02000 + 0x108;
	__REG(reg) = 0x132;
	
	reg = 0x00a02000 + 0x10c;
	__REG(reg) = 0x132;
	
	reg = 0x00a02000 + 0xf60;
	__REG(reg) = 0x40800000;
	
	reg = 0x00a02000 + 0xf80;
	__REG(reg) = 0x3;
	
	
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
	CP15_SET_MVBAR(&monitor_vetor_table);
	
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
						: "r" (arg_r0), "r" (arg_r1), "r" (arg_r2), "r" (arg_r3)
						: "ip", "lr", "memory", "cc");

	while(1) {
		led_ctrl(LED_ON);
		arg_r0 = ID_RUN_PRO;
		__asm__ volatile (".arch_extension sec\n\t"                               
						"dsb\n\t"                                               
						"smc #0\n\t"                                            
						: "=r" (arg_r0)                                         
						: "r" (arg_r0), "r" (arg_r1), "r" (arg_r2), "r" (arg_r3)
						: "ip", "lr", "memory", "cc");
		//asm volatile ("smc #0\n\t");		
	}
	
	return 0;
}

u32 invoke_cmd(u32 cmd, u32 value, u32 dreg, u32 reserve)
{
	__REG(dreg) = value;
	
	return 0;
}