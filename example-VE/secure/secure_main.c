typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define LED_ON	1
#define LED_OFF	0

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

	//(*((void(*)(void))0x0090b020))();
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
