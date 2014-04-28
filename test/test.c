typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define LED_ON	1
#define LED_OFF	0

#define Asm __asm__ volatile
#define CP15_GET_SCR(x) Asm("mrc p15, 0, %0, c1, c1, 0":"=r"(x))
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

int main(void)
{
	unsigned int csu_reg, i;
	csu_reg = 0x020e0028;
	__REG(csu_reg) = 0x00003810;	

	for(i=0; i<10; i++){
		led_ctrl(LED_OFF);
		asm volatile ("smc #0\n\t");
	}
	
	return 0;
}
