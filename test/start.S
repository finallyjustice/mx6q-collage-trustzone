#define CPSR_IRQ_BIT  (0x80)
#define CPSR_FIQ_BIT  (0x40)
#define CPSR_SVC      (0x13)

    .text
    .align  5
    .global _start
_start:
    msr  cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
    
	mov ip, sp
	stmfd sp!, {fp, ip, lr, pc}
	sub fp, ip, #4
	
    bl   main

	sub sp, fp, #12
	ldmfd sp, {fp, sp, pc}

	
	.text
	.align  5
	.arm
	.global _smc_invoke
_smc_invoke:
	ldr r0, =0xe0
	smc #0
	bx lr