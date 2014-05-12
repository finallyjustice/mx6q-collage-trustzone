#define CPSR_IRQ_BIT  (0x80)
#define CPSR_FIQ_BIT  (0x40)
#define CPSR_SVC      (0x13)
#define CPSR_MON      (0x16)

	.text
	.align 5
	.arm
	.global first_go
first_go:
	b _start

	.section .vector, "ax"
	.align 5
	.global monitor_vector_table
monitor_vector_table:
	@ Monitor's
	NOP     @ Reset      - not used by Monitor
	NOP     @ Undef      - not used by Monitor
	B       SMC_Handler
	NOP     @ Prefetch   - can by used by Monitor
	NOP     @ Data abort - can by used by Monitor
	NOP     @ RESERVED
	NOP     @ IRQ        - can by used by Monitor
	NOP     @ FIQ        - can by used by Monitor
	
	.text
	.align 5
	.global SMC_Handler
SMC_Handler:
	stmdb sp!, {r0-r12, lr}
@	srsdb sp!, #CPSR_MON
	
	MRC     p15, 0, r9, c1, c1, 0
	EOR     r9, r9, #1
	MCR     p15, 0, r9, c1, c1, 0
	
	ldmia sp!, {r0-r12, pc}^
@	rfeia sp!

    .text
    .align  5
    .global _start
_start:    
	stmdb sp!, {r0-r12, lr}

	msr  cpsr, #(CPSR_MON|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
	ldr sp, =0x00910000
	
	ldr r0, =monitor_vector_table
	mcr p15, 0, r0, c12, c0, 1
	
	@ NSACR
	ldr r0, =0x00070c00
	mcr p15, 0, r0, c1,  c1, 2
	
	@ CPACR
@	mrc p15, 0, r0, c1,  c0, 2
@	orr r0, r0, #0x00f00000
	ldr r0, =0x00f00000
	mcr p15, 0, r0, c1,  c0, 2
	
	@ SCR
@	mrc p15, 0, r0, c1,  c1, 0
@	orr r0, r0, #0x00000030
	ldr r0, =0x00000030
	mcr p15, 0, r0, c1,  c1, 0
	
@	msr  cpsr, #(CPSR_SVC|CPSR_FIQ_BIT|CPSR_IRQ_BIT)
	cps #CPSR_SVC
	
	bl trustzone_setting
	
	smc #0
		
	ldmia sp!, {r0-r12, pc}

LED_OFF:
	ldr r0, =0x020b4004
	ldr r1, =0x3
	str r1, [r0]
	mov pc, lr
	
LED_ON:
	ldr r0, =0x020b4004
	ldr r1, =0x0
	str r1, [r0]
	bx lr