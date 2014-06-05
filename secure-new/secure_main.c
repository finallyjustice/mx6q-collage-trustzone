#include "config.h"
#include "asm-inc.h"

/* .bss section variables */
/**
 * @brief User stack
 */
u32 user_stack[MAX_CORES][STACK_SIZE]  __attribute__ ((aligned (4)));

/**
 * @brief Supervisor stack
 */
u32 service_stack[MAX_CORES][STACK_SIZE]  __attribute__ ((aligned (4)));

/**
 * @brief Abort stack
 */
u32 abort_stack[MAX_CORES][STACK_SIZE]    __attribute__ ((aligned (4)));

/**
 * @brief Undefined stack
 */
u32 undefined_stack[MAX_CORES][STACK_SIZE]   __attribute__ ((aligned (4)));

/**
 * @brief IRQ stack
 */
u32 irq_stack[MAX_CORES][STACK_SIZE]      __attribute__ ((aligned (4)));

/**
 * @brief FIQ stack
 */
u32 fiq_stack[MAX_CORES][STACK_SIZE]      __attribute__ ((aligned (4)));

/**
 * @brief Monitor stack
 */
u32 monitor_stack[MAX_CORES][STACK_SIZE]  __attribute__ ((aligned (4)));

/**
 * @brief Parameters stack which is used to SMC call parameters
 */
u32 params_stack[PARAM_STACK_SIZE] __attribute__ ((section (".bss"))) 
                                                 __attribute__ ((aligned (4)));



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

void display_support_info(void)
{
	u32 ID_PFR1;

	CP15_GET_ID_PFR1(ID_PFR1);
	if( 0 != ((ID_PFR1 >> 4) & 0xf) )
		uart_puts("====this target support security extensions\n");
	else
		uart_puts("====this target not support security extensions\n");

	if( 0 != ((ID_PFR1 >> 12) & 0xf) )
		uart_puts("====this target support virtualization extensions\n");
	else
		uart_puts("====this target not support virtualization extensions\n");
}

void target_init(void)
{
	cpu_init();
	uart_init();
	uart_puts("\n");
	display_support_info();

	CP15_SET_SCR(0x30);
	CP15_SET_NSACR(0x00073fff);
	CP15_SET_CPACR(0x0fffffff); // Full access for cp11 and cp10

	target_csu_init();
	
	target_scu_init();

	target_gic_init();
}

void secure_loop(void)
{
	register u32 r0 asm("r0") = 0;
    register u32 r1 asm("r1") = 0;
    register u32 r2 asm("r2") = 0;
	register u32 r3 asm("r3") = 0;

	u32 value;
	
	while(1){
		uart_puts("====enter the secure world\n");

		if( SMCID_NS_INVOKE_S == params_stack[0] ){
			switch(params_stack[1]){
			case 0xF000:
				uart_puts("====invoke cp15 0, c1, c0, 1 write\n");
				value = 0x41;
				asm volatile("mcr p15, 0, %0, c1,  c0,  1\n\t"::"r"(value):);
				//asm volatile("mcr p15, 0, %0, c1,  c0,  1\n\t"::"r"(params_stack[2]):"memory", "cc");
				break;
					
			case 0xF004:
				uart_puts("====invoke cp15 0, c15, c0, 1 write\n");
				//asm volatile("mcr p15, 0, %0, c15, c0,  1\n\t"::"r"(params_stack[2]):"memory", "cc");
				break;

			case 0xF008:
				uart_puts("====invoke register write\n");
				__REG(params_stack[2]) = params_stack[3];
				break;

			default:
				uart_puts("nothing to do in secure world\n");
				break;
			}
		}

		uart_puts("====return to normal world\n");
		
		r0 = SMCID_SWITCH_TO_NS;
		r1 = 0;
		r2 = 0;
		r3 = 0;
		asm volatile(".arch_extension sec\n\t"
					"dsb\n\t"
					"smc #0\n\t" 
					: 
					:"r"(r0),"r"(r1),"r"(r2),"r"(r3)
					:"memory", "cc");
	}
}

void secure_exception_handler(u32 type)
{
	switch(type){
	case 0x0:
		uart_puts("====secure_reset_handler\n");
		break;
	case 0x4:
		uart_puts("====secure_undefined_handler\n");
		break;
	case 0x8:
		uart_puts("====secure_swi_handler\n");
		break;
	case 0xc:
		uart_puts("====secure_prefetch_handler\n");
		break;
	case 0x10:
		uart_puts("====secure_abort_handler\n");
		break;
	case 0x14:
		uart_puts("====secure_hypvisor_handler\n");
		break;
	case 0x18:
		uart_puts("====secure_irq_handler\n");
		break;
	case 0x1c:
		uart_puts("====secure_fiq_handler\n");
		break;
	default:
		uart_puts("====secure exception error\n");
		break;
	
	}
}

void monitor_exception_handler(u32 type)
{
	switch(type){
	case 0x0:
		uart_puts("====monitor_reset_handler\n");
		break;
	case 0x4:
		uart_puts("====monitor_undefined_handler\n");
		break;
	case 0x8:
		uart_puts("====monitor_smc_handler\n");
		break;
	case 0xc:
		uart_puts("====monitor_prefetch_handler\n");
		break;
	case 0x10:
		uart_puts("====monitor_abort_handler\n");
		break;
	case 0x14:
		uart_puts("====monitor_hypvisor_handler\n");
		break;
	case 0x18:
		uart_puts("====monitor_irq_handler\n");
		break;
	case 0x1c:
		uart_puts("====monitor_fiq_handler\n");
		break;
	default:
		uart_puts("====monitor exception error\n");
		break;
	
	}
}


