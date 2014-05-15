#define ID_RUN_UBOOT		(0xF0)
#define ID_SWITCH_TO_NS		(0xF4)
#define ID_SWITCH_TO_S		(0xF8)
#define ID_EXE_CMD			(0xFC)
#define ID_RUN_PRO			(0xE0)
#define ID_EXE_CP15_WR		(0xE4)

#define ns_image 0x27800000

#define cpsr_mode_svc	(0x13)
#define cpsr_mode_mon	(0x16)
#define cpsr_irq_bit	(0x80)
#define cpsr_fiq_bit	(0x40)

.equ NS_BIT,	0x1

@ ------------------------------------------------------------
@ Monitor mode vector table
@ ------------------------------------------------------------
	.section .vector, "ax"
	.global monitor_vetor_table
	.align 5
monitor_vetor_table:
	@ Monitor's
	NOP     @ Reset      - not used by Monitor
	NOP     @ Undef      - not used by Monitor
	B       SMC_Handler
	NOP     @ Prefetch   - can by used by Monitor
	NOP     @ Data abort - can by used by Monitor
	NOP     @ RESERVED
	NOP     @ IRQ        - can by used by Monitor
	NOP     @ FIQ        - can by used by Monitor

@ ------------------------------------------------------------
@ SMC Handler
@
@ - Detect which world executed SMC
@ - Saves state to appropiate stack
@ - Restores other worlds state
@ - Switches world
@ - Performs exception return
@ ------------------------------------------------------------
	.global SMC_Handler
SMC_Handler:
	cmp r0, #ID_EXE_CMD
	beq exe_cmd
	
	cmp r0, #ID_EXE_CP15_WR
	beq exe_cp15_wr
	
	cmp	r0, #ID_RUN_UBOOT
	beq uboot_setting

skip_uboot_setting:
	PUSH   {r0-r3}
		
	MRC     p15, 0, r9, c1, c1, 0        @ Read Secure Configuration Register data
	TST     r9, #NS_BIT                  @ Is the NS bit set?
	EOR     r9, r9, #NS_BIT              @ Toggle NS bit
	MCR     p15, 0, r9, c1, c1, 0        @ Write Secure Configuration Register data
	
	@ Load save to pointer
	@ ---------------------
	LDREQ   r0, =S_STACK_SP             @ If NS bit set, was in Normal world.  So restore Secure state
	LDRNE   r0, =NS_STACK_SP
	LDR     r2, [r0]
	
	@ Load restore from pointer
	@ --------------------------
	LDREQ   r1, =NS_STACK_SP
	LDRNE   r1, =S_STACK_SP
	LDR     r3, [r1]
	
	@ r2  <-- save to
	@ r3  <-- restore from
	
	@ Save general purpose registers, SPSR and LR
	@ --------------------------------------------
	STMFD   r2!, {r4-r12}               @ Save r4 to r12
	@ ADD SUPPORT FOR SPs
	MRS     r4, spsr                    @ Also get a copy of the SPSR
	STMFD   r2!, {r4, lr}               @ Save original SPSR and LR
	STR     r2, [r0]                    @ Save updated pointer back, r0 and r2 now free
	
	@ Restore other world's registers, SPSR and LR
	@ ---------------------------------------------
	LDMFD   r3!, {r0, lr}               @ Get SPSR and LR from
	@ ADD SUPPORT FOR SPs
	MSR     spsr_cxsf, r0               @ Restore SPSR
	LDMFD   r3!, {r4-r12}               @ Restore registers r4 to r12
	STR     r3, [r1]                    @ Save updated pointer back, r1 and r3 now free
	
	CLREX
	
	POP     {r0-r3}
	MOVS    pc, lr
	
uboot_setting: 
	/* Prepare the first NT OS start address PC and CPSR */
	LDR     r0, =NS_STACK_LIMIT         
	STMFD   r0!, {r4-r12}              
	@ ADD support for SPs
	ldr r1, =(cpsr_mode_svc | cpsr_irq_bit | cpsr_fiq_bit)
	ldr r2, =ns_image
	STMFD   r0!, {r1, r2}
	
	LDR     r1, =NS_STACK_SP             @ Get address of global
	STR     r0, [r1]                     @ Save non-secure stack pointer
	
	LDR     r0, =S_STACK_LIMIT
	LDR     r1, =S_STACK_SP              @ Get address of global
	STR     r0, [r1]                     @ Save secure stack pointer
		
	b	skip_uboot_setting
	
exe_cmd:
@	srsdb sp!, #cpsr_mode_mon
	stmdb sp!, {r0-r12, lr}
	bl invoke_cmd
@	rfeia sp!
	ldmia sp!, {r0-r12, pc}^
	
exe_cp15_wr:
	srsdb sp!, #cpsr_mode_mon	
	mcr  p15, 0, r1, c15, c0, 1
	rfeia sp!

@ led off	
@	ldr r8, =0x020b4004
@	ldr r7, =0x3
@	str r7, [r8]	
	
@ ------------------------------------------------------------
@ Space reserved for stacks
@ ------------------------------------------------------------  
NS_STACK_BASE:
  .word     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
NS_STACK_LIMIT:

S_STACK_BASE:
  .word     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
S_STACK_LIMIT:

NS_STACK_SP:
  .word     0

S_STACK_SP:
  .word     0

@ ------------------------------------------------------------
@ End
@ ------------------------------------------------------------	
