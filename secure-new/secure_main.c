typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define Asm __asm__ volatile
#define CP15_SET_VBAR(x)	Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_SET_MVBAR(x)	Asm("mcr p15, 0, %0, c12, c0, 1"::"r"(x))
#define CP15_SET_SCR(x)		Asm("mcr p15, 0, %0, c1,  c1, 0"::"r"(x))
#define CP15_SET_NSACR(x)	Asm("mcr p15, 0, %0, c1,  c1, 2"::"r"(x))
#define CP15_SET_CPACR(x)	Asm("mcr p15, 0, %0, c1,  c0, 2"::"r"(x))

#define CP15_GET_SCR(x)		Asm("mrc p15, 0, %0, c1,  c1, 0":"=r"(x))

//For Secure Configuration Register
//=================================
#define SCR_NET	(1 << 6)	/*Not Early Termination*/
#define SCR_AW	(1 << 5)	/*0  the CPSR.A bit can be modified only in Secure state.*/
							/*1  the CPSR.A bit can be modified in any security state.*/
#define SCR_FW	(1 << 4)	/*0  the CPSR.F bit can be modified only in Secure state*/
							/*1  the CPSR.F bit can be modified in any security state.*/
#define SCR_EA	(1 << 3)	/*0  Abort mode handles external aborts*/
							/*1  Monitor mode handles external aborts.*/
#define SCR_FIQ	(1 << 2)	/*0  FIQ mode entered when FIQ is taken*/
							/*1  Monitor mode entered when FIQ is taken.*/
#define SCR_IRQ	(1 << 1)	/*0  IRQ mode entered when IRQ is taken*/
							/*1  Monitor mode entered when IRQ is taken.*/
#define SCR_NS	(1 << 0)	/*0:Secure state. 1:Non-secure state*/

#define SCR_SETTING	( SCR_AW | SCR_FW )
//=================================SCR

//For Non-Secure Access Control Register
//=================================
#define NSACR_NS_SMP_BIT	(1 << 18)
/*Determines if the SMP bit of the Auxiliary Control Register is writeable in Non-secure state*/

#define NSACR_TL_BIT		(1 << 17)
/*Determines if lockable TLB entries can be allocated in Non-secure state*/

#define NSACR_PLE_BIT		(1 << 16)
/*Controls NS accesses to the Preload Engine resources*/

#define NSACR_CP11_BIT		(1 << 11)
/*Determines permission to access coprocessor 11 in the Non-secure state*/

#define NSACR_CP10_BIT		(1 << 10)
/*Determines permission to access coprocessor 10 in the Non-secure state*/

#define NSACR_SETTING (NSACR_NS_SMP_BIT | NSACR_TL_BIT |NSACR_PLE_BIT| NSACR_CP11_BIT | NSACR_CP10_BIT)
//=================================end NSACR

#define ID_RUN_UBOOT		(0xF0)
#define ID_SWITCH_TO_NS		(0xF4)
#define ID_SWITCH_TO_S		(0xF8)
#define ID_EXE_CMD			(0xFC)
#define ID_RUN_PRO			(0xE0)
#define ID_EXE_CP15_WR		(0xE4)

extern u32 monitor_vetor_table;

void delay(void)
{
	u32 i;
	for( i=0; i<0x100000; i++ );
}

#define LED_ON	1
#define LED_OFF	0
void led_ctrl(u32 led_flag)
{
	delay();

    if( LED_ON == led_flag ){
        __REG(0x020b4004) = 0x0;
    }
    else {
        __REG(0x020b4004) = 0x3;
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
	__REG(0x020e0028) = 0x00003810; 
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
#define ICDBASE		0x00a01000
#define ICDDCR		0x000
#define ICDICTR		0x004
#define ICDISR0		0x080
#define ICDSGIR		0xf00

#define ICCBASE		0x00a00100
#define ICCICR		0x00
#define ICCPMR		0x04
#define ICCBPR		0x08
#define ICCABPR		0x1c

void target_gic_init(void)
{
	u32 i;

	u32 ITLinesNumber = (*((u32*)(ICDBASE+ICDICTR))) & 0x1f;
	
	// ICDISR Secure Interrupt Security Registers
	// cortex-a9 only use PPI(31-27), so setting PPI(31-27) SGI(0-7) non-secure
	__REG(ICDBASE + ICDISR0) = 0xf80000ff;
	
	// set all SPI non-secure, depend on Interrupt Controller Type Register (ICDICTR)ITLinesNumber
	for(i = 1; i <= (ITLinesNumber+1); i++)
		__REG(ICDBASE + ICDISR0 + 4*i) = 0xffffffff;

	// ICDDCR Banked Distributor Control Register
	//__REG(ICDBASE + ICDDCR) = 0x1;
	
	// ICCPMR Interrupt Priority Mask Register
	__REG(ICCBASE + ICCPMR) = 0xf8;

	// ICCICR Banked CPU Interface Control Register
	//__REG(ICCBASE + ICCICR) = 0x1f;
}

void target_init(void)
{
	u32 i;
	char* dest = (char*)0x27800000;
	char* src  = (char*)0x0090b000;	
	
	CP15_SET_MVBAR(&monitor_vetor_table);
	
	CP15_SET_SCR(0x30);
	
	CP15_SET_NSACR(0x00073fff);
	
	CP15_SET_CPACR(0x0fffffff); // Full access for cp11 and cp10

	target_csu_init();
	
	target_scu_init();

	target_gic_init();

	// copy uboot from iram to sadram
	for(i=0; i<160096; i++)
		*dest++ = *src++;

#if 0	
	//start uboot directly
	(*((void(*)(void))0x27800000))(); 
#endif	
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

u32 invoke_reg_wr(u32 cmd, u32 value, u32 dreg, u32 reserve)
{
	__REG(dreg) = value;
	
	return 0;
}

u32 invoke_cp15_wr(u32 cmd, u32 type, u32 value, u32 reserve)
{

	switch(type){
	case 0xF000: //ACTLR
		asm volatile("mcr p15, 0, %0, c1,  c0,  1\n\t"::"r"(value):"memory", "cc");
		break;
	case 0xF004: 
		asm volatile("mcr p15, 0, %0, c15, c0,  1\n\t"::"r"(value):"memory", "cc");
		break;

	default:
		break;
	}
	
	return value;
}