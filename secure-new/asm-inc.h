#define cpsr_mode_use		(0x10)
#define cpsr_mode_fiq		(0x11)
#define cpsr_mode_irq		(0x12)
#define cpsr_mode_svc		(0x13)
#define cpsr_mode_mon		(0x16)
#define cpsr_mode_abt		(0x17)
#define cpsr_mode_und		(0x1b)
#define cpsr_mode_sys		(0x1F)

#define cpsr_irq_bit		(0x80)
#define cpsr_fiq_bit		(0x40)
#define cpsr_asyc_bit		(0x100)

#define SMCID_SWITCH_UBOOT	(0xFFFFFF00)
#define SMCID_SWITCH_TO_NS	(0xFFFFFF04)
#define SMCID_SWITCH_TO_S	(0xFFFFFF08)
#define SMCID_NS_INVOKE_S	(0xFFFFFF0C)
#define SMCID_RET_S_TO_NS	(0xFFFFFF10)

#define SCR_NS_BIT			(0x1)

#define STACK_SIZE			(4096)
#define MAX_CORES			(1)
#define PARAM_STACK_SIZE	(8)

#define BASE_ADDR_STACK		(0x00908000)	// secure ram
#define MODE_STACK_SIZE		(0x1000)

#define SVC_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*0)
#define IRQ_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*1)
#define FIQ_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*2)
#define UND_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*3)
#define SYS_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*4)
#define ABT_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*5)
#define MON_MODE_STACK_BASE	(BASE_ADDR_STACK + MODE_STACK_SIZE*6)



