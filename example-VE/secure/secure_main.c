typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define LED_ON	1
#define LED_OFF	0

#define Asm __asm__ volatile
#define CP15_SET_NSACR(x)	Asm("mcr p15, 0, %0, c1, c1, 2"::"r"(x))
#define CP15_SET_CPACR(x)	Asm("mcr p15, 0, %0, c1, c0, 2"::"r"(x))
#define CP15_GET_SCR(x)		Asm("mrc p15, 0, %0, c1, c1, 0":"=r"(x))
#define CP15_SET_SCR(x)		Asm("mrc p15, 0, %0, c1, c1, 0"::"r"(x))

extern void monitorInit();

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

int secure_main(void)
{
	unsigned int csu_reg, i;
	for(csu_reg = 0x021C0000; csu_reg < 0x021C00A0; csu_reg = csu_reg + 4)
		__REG(csu_reg) = 0x00ff00ff; //0x00330033 for Peripheral access policy
		
	// for CORAM secure policy setting
	//csu_reg = 0x021C0000 + 0x**;
	//__REG(csu_reg) = 0x00330033;
	
	// for OCRAM secure status setting
	csu_reg = 0x020e0028;
	__REG(csu_reg) = 0x00003810;
	
	// set for non-secure can access some coprocessor reg
	CP15_SET_NSACR(0x00073fff);
	CP15_SET_CPACR(0x0fffffff);
	
	// set for SCR
	CP15_SET_SCR(0b110000);

	//(*((void(*)(void))0x0090b020))();
	// copy uboot from iram to sadram
	char* dest = (char*)0x27800000;
	char* src  = (char*)0x0090b000;
	for(i=0; i<160096; i++)
		*dest++ = *src++;
	
	// Install monitor
	monitorInit();
	
	while(1) {
		led_ctrl(LED_ON);
		asm volatile ("smc #0\n\t");
	}
	
	return 0;
}
