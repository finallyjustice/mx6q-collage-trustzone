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

#define CP15_SET_SCR(x)		Asm("mrc p15, 0, %0, c1,  c1, 0"::"r"(x))
#define CP15_SET_VBAR(x) 	Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define ISB() Asm("isb")

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
	
	CP15_GET_NSACR(reg);
	printf("NSACR = %08x\n", reg);
	
	CP15_GET_CPACR(reg);
	printf("CPACR = %08x\n", reg);
		
	//CP15_GET_SCR(reg);
	//printf("SCR = %08x\n", reg);
	
	//ISB();
	while(1){
		led_ctrl(LED_OFF);
		asm volatile ("smc #0\n\t");
	}
	
	return 0;
}
