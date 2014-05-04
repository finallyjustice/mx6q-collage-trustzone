typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define LED_ON	1
#define LED_OFF	0

#define Asm __asm__ volatile
#define CP15_GET_SCR(x) 	Asm("mrc p15, 0, %0, c1, c1, 0":"=r"(x))
#define CP15_GET_NSACR(x) 	Asm("mrc p15, 0, %0, c1, c1, 2":"=r"(x))
#define CP15_GET_CPACR(x) 	Asm("mrc p15, 0, %0, c1, c0, 2":"=r"(x))

#define CP15_SET_SCR(x)		Asm("mcr p15, 0, %0, c1,  c1, 0"::"r"(x))
#define CP15_SET_VBAR(x) 	Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define ISB() Asm("isb")

#define ID_RUN_UBOOT		(0xF0)
#define ID_SWITCH_TO_NS		(0xF4)
#define ID_SWITCH_TO_S		(0xF8)
#define ID_EXE_CMD			(0xFC)
#define ID_RUN_PRO			(0xE0)

extern void _smc_invoke();

void led_ctrl(u32 led_flag)
{
    u32 led_reg, i;
    for( i=0; i<0x100000; i++ );

    led_reg = 0x020b4004;
    if( LED_ON == led_flag ){
        __REG(led_reg) = 0x0;
    }
    else {
        __REG(led_reg) = 0x3;
    }
}

void (*printf)(char *,...) = 0x27812cd8;

int main(void)
{
	unsigned int reg;
	
	register unsigned int arg_r0 asm("r0") = ID_EXE_CMD;
	register unsigned int arg_r1 asm("r1") = 2;
	register unsigned int arg_r2 asm("r2") = 3;
	register unsigned int arg_r3 asm("r3") = 4;
	
	
	CP15_GET_NSACR(reg);
	printf("NSACR = %08x\n", reg);
	
	CP15_GET_CPACR(reg);
	printf("CPACR = %08x\n", reg);
	
		
	//CP15_GET_SCR(reg);
	//printf("SCR = %08x\n", reg);
	
	//ISB();
	while(1){
		led_ctrl(LED_OFF);
		printf("invoke smc\n");
		//_smc_invoke();
		//asm volatile ("smc #0\n\t");
		
		arg_r0 = ID_EXE_CMD;
		__asm__ volatile (".arch_extension sec\n\t"                               
						"dsb\n\t"                                               
						"smc #0\n\t"                                            
						: "=r" (arg_r0)                                         
						: "r" (arg_r0), "r" (arg_r1), "r" (arg_r2), "r" (arg_r3)
						: "r4", "r5", "r6", "r7", "r8", "r9", "r10", "ip", "lr", \
						"memory", "cc");
				
	}
	
	return 0;
}
