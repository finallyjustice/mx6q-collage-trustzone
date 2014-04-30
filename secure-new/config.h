typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define LED_ON	1
#define LED_OFF	0

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

#define NSACR_SETTING (NSACR_NS_SMP_BIT | NSACR_TL_BIT | NSACR_PLE_BIT | NSACR_CP11_BIT | NSACR_CP10_BIT)
//=================================end NSACR

#define ID_RUN_UBOOT		(0xF0)
#define ID_SWITCH_TO_NS		(0xF1)
#define ID_SWITCH_TO_S		(0xF2)
#define ID_EXE_CMD			(0xF3)
#define ID_RUN_PRO			(0xF4)

